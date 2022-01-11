#include <TaskScheduler.h>
#include <SimpleDHT.h>
#include <QuickPID.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/*   Configurazione   */

#define RELE_POMPA D1
#define ATOMIZZATORE D2
#define SENSORE_TERRA_ENABLE D3
#define LED_STRIP D4
#define FOTORESISTENZA_ENABLE D5
#define PIASTRA D6
#define FOTORESISTENZA A0   /* LETTURE*/
#define SENSORE_TERRA A0    /* "MULTIPLEXATE" su A0*/
#define DHT11_PIN D7

SimpleDHT11 dht11(DHT11_PIN);


#define TEMPO_EROGAZIONE 5000 // In millisecondi


void pwm_resistenza( );
void modalita_luce( );
void accendi_pompa( );
void spegni_pompa( );
void update_tempo( );
void pwm_atom( );
void pwm_led( );
void pwm_res( );
void leggi( );
void sync( );


/*   Globali   */

const char *ssid     = "YOUR SSID";
const char *password = "YOUR PASSWORD";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int ore, minuti, secondi;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

long timer_pompa;
Scheduler scheduler;

float kp_led = 0.1, ki_led = 0.1, kd_led = 0;                 //
float kp_piastra = 0.5, ki_piastra = 0.5, kd_piastra = 0.5;   // COSTANTI PID
float kp_neb = 0.5, ki_neb = 0.5, kd_neb = 0.5;               //

float luce_setpoint = 600, luce_in, luce_out;                       //
float temperatura_setpoint = 23.0, temperatura_in, temperatura_out; // Setpoint, variabili di input e output
float umid_aria_setpoint, umid_aria_in, umid_aria_out;              //

QuickPID PID_led(&luce_in, &luce_out, &luce_setpoint);
QuickPID PID_piastra(&temperatura_in, &temperatura_out, &temperatura_setpoint);
QuickPID PID_neb(&umid_aria_in, &umid_aria_out, &umid_aria_setpoint);


/*   TASK   */

Task modalita_notte(1 * TASK_MINUTE, TASK_FOREVER, modalita_luce);
Task aggiorna_orario(1 * TASK_MINUTE, TASK_FOREVER, update_tempo);
Task sincronizza_ora(24 * TASK_HOUR, TASK_FOREVER, sync);
Task controllo_pompa(3 * TASK_HOUR, TASK_FOREVER, accendi_pompa);
Task controllo_res(5 * TASK_SECOND, TASK_FOREVER, pwm_res);
Task stop_pompa(2 * TASK_SECOND, TASK_FOREVER, spegni_pompa);
Task controllo_atom(1 * TASK_SECOND, TASK_FOREVER, pwm_atom);
Task controllo_led(1 * TASK_SECOND, TASK_FOREVER, pwm_led);
Task leggi_DHT(1 * TASK_SECOND, TASK_FOREVER, leggi);


void setup() {

  Serial.begin(9600);

  //digitalWrite(RELE_POMPA, HIGH);
  pinMode(RELE_POMPA, OUTPUT);
  pinMode(ATOMIZZATORE, OUTPUT);
  pinMode(SENSORE_TERRA_ENABLE, OUTPUT);
  pinMode(FOTORESISTENZA_ENABLE, OUTPUT);
  pinMode(LED_STRIP, OUTPUT);
  pinMode(FOTORESISTENZA, INPUT);
  pinMode(SENSORE_TERRA, INPUT);

  PID_led.SetTunings(kp_led, ki_led, kd_led);
  PID_led.SetMode(PID_led.Control::automatic);
  PID_led.SetOutputLimits(0, 230);

  PID_piastra.SetTunings(kp_piastra, ki_piastra, kd_piastra);
  PID_piastra.SetMode(PID_piastra.Control::automatic);
  PID_piastra.SetOutputLimits(0, 190);                // Evito di scaldare troppo la piastra

  PID_neb.SetTunings(kp_neb, ki_neb, kd_neb);
  PID_neb.SetMode(PID_neb.Control::automatic);

  scheduler.init( );

  scheduler.addTask(controllo_pompa);
  controllo_pompa.enable( ); 

  scheduler.addTask(controllo_res);
  controllo_res.enable( );

  scheduler.addTask(stop_pompa);
  stop_pompa.disable( );

  scheduler.addTask(controllo_atom);
  controllo_atom.enable( );

  scheduler.addTask(controllo_led);
  //controllo_led.enable( );

  scheduler.addTask(leggi_DHT);
  leggi_DHT.enable( );
 
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

}

void loop() {

  scheduler.execute( );

}


void accendi_pompa( ) {

  digitalWrite(SENSORE_TERRA_ENABLE, HIGH);
  int lettura = analogRead(SENSORE_TERRA);
  if (lettura < 300 /*DA TESTARE*/) {
    digitalWrite(RELE_POMPA, LOW);
    timer_pompa = millis( );
    stop_pompa.enable( );
  }
  digitalWrite(SENSORE_TERRA_ENABLE, LOW);
}

void spegni_pompa( ) {

  if ((millis( ) - timer_pompa) >= TEMPO_EROGAZIONE) {
    Serial.println(F("SPEGNENDO POMPA"));
    digitalWrite(RELE_POMPA, HIGH);
    stop_pompa.disable( );
  }
}

void pwm_res( ) {

  PID_piastra.Compute( );
  analogWrite(PIASTRA, temperatura_setpoint);

}

void pwm_atom( ) {

  PID_neb.Compute( );
  analogWrite(ATOMIZZATORE, umid_aria_out);

}

void pwm_led( ) {

  digitalWrite(FOTORESISTENZA_ENABLE, HIGH);
  luce_in = analogRead(FOTORESISTENZA);
  PID_led.Compute( );
  analogWrite(LED_STRIP, luce_out);
  digitalWrite(FOTORESISTENZA_ENABLE, LOW);

}

void leggi( ) {

  byte temp = 0, humid = 0;
  int err = SimpleDHTErrSuccess;
  if((err = dht11.read(&temp, &humid, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Lettura DHT11 fallita, errore="); Serial.print(SimpleDHTErrCode(err));
    return;
  }
  
  temperatura_in = ((float) temp);
  umid_aria_in = ((float) humid);
  Serial.print("Sample OK: ");
  Serial.printf("Temperatura: %d *C\n", (int) temp);
  Serial.printf("Umidita: %d %\n", (int) humid);
  
}

void sync( ) {
  timeClient.update( );
  ore = timeClient.getHours( );
  minuti = timeClient.getMinutes( );
  secondi = timeClient.getSeconds( );
}

void modalita_luce( ) {

  if ((ore > 20 || ore < 7) && !controllo_led.isEnabled( ))
    controllo_led.enable( );
  else if ((ore > 6 && ore < 21) && controllo_led.isEnabled( ))
    controllo_led.disable( );

}

void update_tempo( ) {

  minuti = (minuti + 1) % 60;
  if (!minuti) ore = (ore + 1) % 24;

}
