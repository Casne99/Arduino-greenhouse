#include <TaskScheduler.h>
#include <SimpleDHT.h>
#include <QuickPID.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/*   Configurazione   */

#define RELE_POMPA D1
#define ATOMIZZATORE D2
#define LED_STRIP D4
#define FOTORESISTENZA_ENABLE D5
#define PIASTRA D6
#define FOTORESISTENZA A0
#define DHT11_PIN D7

SimpleDHT11 dht11(DHT11_PIN);


#define TEMPO_EROGAZIONE 1000 // In millisecondi


/* Prototipi funzioni */

void pwm_resistenza( );
void modalita_luce( );
void accendi_pompa( );
void spegni_pompa( );
void update_tempo( );
void pwm_atom( );
void pwm_led( );
void pwm_res( );
void leggi( );
void print( );
void sync( );


/*   Globali   */

const char *ssid     = /*your ssid*/
const char *password = /*your password*/

const long utcOffsetInSeconds = 3600; // sfasamento italiano rispetto a GMT in secondi

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int ore = 0, minuti = 0, secondi = 0;

// Definisco client NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

long timer_pompa = 0;
Scheduler scheduler;

float kp_led = 0.1, ki_led = 0, kd_led = 0;                   //
float kp_piastra = 3.5, ki_piastra = 0.5, kd_piastra = 1.3;   // COSTANTI PID
float kp_neb = 1, ki_neb = 0.1, kd_neb = 0.5;                 //

float luce_setpoint = 475, luce_in, luce_out;                       //
float temperatura_setpoint = 24.0, temperatura_in, temperatura_out; // Setpoint, variabili di input e output
float umid_aria_setpoint = 65.0, umid_aria_in, umid_aria_out;       //

QuickPID PID_led(&luce_in, &luce_out, &luce_setpoint);
QuickPID PID_piastra(&temperatura_in, &temperatura_out, &temperatura_setpoint);
QuickPID PID_neb(&umid_aria_in, &umid_aria_out, &umid_aria_setpoint);

/*   TASK   */

//Task modalita_notte(1 * TASK_MINUTE, TASK_FOREVER, modalita_luce);
Task aggiorna_orario(1 * TASK_MINUTE, TASK_FOREVER, update_tempo);
Task sincronizza_ora(6 * TASK_HOUR, TASK_FOREVER, sync);
Task controllo_pompa(/*4 * TASK_HOUR*/ 50 * TASK_SECOND, TASK_FOREVER, accendi_pompa);
Task controllo_res(0.2 * TASK_SECOND, TASK_FOREVER, pwm_res);
Task stop_pompa(2 * TASK_SECOND, TASK_FOREVER, spegni_pompa);
Task controllo_atom(0.02 * TASK_SECOND, TASK_FOREVER, pwm_atom);
Task controllo_led(0.1 * TASK_SECOND, TASK_FOREVER, pwm_led);
Task leggi_DHT(2 * TASK_SECOND, TASK_FOREVER, leggi);
Task stampa_output(4 * TASK_SECOND, TASK_FOREVER, print);


void setup_Wifi( ) {
  WiFi.begin(ssid, password);

  Serial.printf("\n\nConnettendo a %s\n", ssid);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connessione stabilita\n\n");
}

void set_PID_parameters( ) {

  PID_led.SetTunings(kp_led, ki_led, kd_led);
  PID_led.SetMode(PID_led.Control::automatic);
  PID_led.SetOutputLimits(0, 230);

  PID_piastra.SetTunings(kp_piastra, ki_piastra, kd_piastra);
  PID_piastra.SetMode(PID_piastra.Control::automatic);
  //PID_piastra.SetOutputLimits(0, 190);                // Evito di scaldare troppo la piastra

  PID_neb.SetTunings(kp_neb, ki_neb, kd_neb);
  PID_neb.SetMode(PID_neb.Control::automatic);
}

void setup_scheduler( ) {
  scheduler.init( );

  scheduler.addTask(sincronizza_ora);
  sincronizza_ora.enable( );

  /*scheduler.addTask(modalita_notte);
  modalita_notte.enable( );*/

  scheduler.addTask(controllo_pompa);
  controllo_pompa.enable( ); 

  scheduler.addTask(controllo_res);
  controllo_res.enable( );

  scheduler.addTask(stop_pompa);
  stop_pompa.disable( );

  scheduler.addTask(controllo_atom);
  controllo_atom.enable( );

  scheduler.addTask(controllo_led);
  controllo_led.enable( );

  scheduler.addTask(leggi_DHT);
  leggi_DHT.enable( );

  scheduler.addTask(aggiorna_orario);
  aggiorna_orario.enable( );

  scheduler.addTask(stampa_output);
  stampa_output.enable( );
}


void setup() {

  digitalWrite(LED_STRIP, LOW);
  digitalWrite(ATOMIZZATORE, LOW);
  digitalWrite(RELE_POMPA, HIGH);
  digitalWrite(FOTORESISTENZA_ENABLE, LOW);
  digitalWrite(PIASTRA, LOW);

  Serial.begin(9600);

  setup_Wifi( );
  set_PID_parameters( );
  setup_scheduler( );
  timeClient.begin( );

  pinMode(RELE_POMPA, OUTPUT);
  pinMode(ATOMIZZATORE, OUTPUT);
  pinMode(FOTORESISTENZA_ENABLE, OUTPUT);
  pinMode(LED_STRIP, OUTPUT);
  pinMode(FOTORESISTENZA, INPUT);

}

void loop() {

  scheduler.execute( );

}


void accendi_pompa( ) {

  Serial.println("ACCENDENDO POMPA");
  digitalWrite(RELE_POMPA, LOW);
  timer_pompa = millis( );
  stop_pompa.enable( );

}

void spegni_pompa( ) {

  if ((millis( ) - timer_pompa) >= TEMPO_EROGAZIONE) {
    Serial.println("SPEGNENDO POMPA");
    digitalWrite(RELE_POMPA, HIGH);
    stop_pompa.disable( );
  }
}

void pwm_res( ) {

  PID_piastra.Compute( );
  analogWrite(PIASTRA, temperatura_out);

}

void pwm_atom( ) {

  PID_neb.Compute( );
  analogWrite(ATOMIZZATORE, umid_aria_out);

}

void pwm_led( ) {

  digitalWrite(FOTORESISTENZA_ENABLE, HIGH);
  luce_in = (float) analogRead(FOTORESISTENZA);
  PID_led.Compute( );
  analogWrite(LED_STRIP, luce_out);
  digitalWrite(FOTORESISTENZA_ENABLE, LOW);

}

void leggi( ) {

  byte temp = 0, humid = 0;
  int err = SimpleDHTErrSuccess;
  if((err = dht11.read(&temp, &humid, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Lettura DHT11 fallita, errore = "); Serial.println(SimpleDHTErrCode(err));
    return;
  }
  
  temperatura_in = ((float) temp);
  umid_aria_in = ((float) humid);
  Serial.printf("Sample OK: \nTemperatura: %d *C\nUmidita: %d %\n", (int) temp, (int) humid);
}

void sync( ) {

  Serial.print("Sincronizzo orario\n");

  timeClient.update( );
  ore = timeClient.getHours( );
  minuti = timeClient.getMinutes( );
  secondi = timeClient.getSeconds( );

  Serial.printf("Orario ufficiale: %d:%d:%d\n\n", ore, minuti, secondi);
}

void modalita_luce( ) {

  if ((ore > 20 || ore < 7) && !controllo_led.isEnabled( ))
    controllo_led.enable( );
  else if ((ore > 6 && ore < 21) && controllo_led.isEnabled( )) {
    controllo_led.disable( );
    analogWrite(LED_STRIP, 0);
  }
}

void update_tempo( ) {

  minuti = (minuti + 1) % 60;
  if (!minuti) ore = (ore + 1) % 24;

  Serial.printf("Orario aggiornato: %d:%d:%d\n\n", ore, minuti, secondi);

}

void print( ) {

  Serial.printf("Temp: %.1f\tHum: %.1f\tLight: %.1f\n", temperatura_out, umid_aria_out, luce_out);

}
