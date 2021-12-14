#include <TaskScheduler.h>

/*   Configurazione   */

#define RELE_POMPA D1
#define ATOMIZZATORE D2
/*#define RELE_LUCI D4 (con interrupt)
#define SENSORE_TERRA D3
#define TR_LED D3*/


#define TEMPO_EROGAZIONE 5000 // In millisecondi

void pwm_resistenza( );
void accendi_pompa( );
void spegni_pompa( );
//void spegnimento( );
void accensione( ); 
void pwm_atom( );
void pwm_led( );
void pwm_res( );
void stampa( );



/*   Globali   */

long timer_pompa;  
Scheduler scheduler;


/*   TASK   */

Task controllo_pompa(3 * TASK_HOUR, TASK_FOREVER, accendi_pompa);
Task controllo_res(2 * TASK_SECOND, TASK_FOREVER, pwm_res);
Task stop_pompa(2 * TASK_SECOND, TASK_FOREVER, spegni_pompa);
Task controllo_atom(2 * TASK_SECOND, TASK_FOREVER, pwm_atom);
Task controllo_led(1 * TASK_SECOND, TASK_FOREVER, pwm_led);
Task stampa_dati(1 * TASK_HOUR, TASK_FOREVER, stampa);
//Task startup(TASK_ONCE, accensione);


void setup() {

  //digitalWrite(RELE_POMPA, HIGH);
  pinMode(RELE_POMPA, OUTPUT);
  pinMode(ATOMIZZATORE, OUTPUT);
  /*pinMode(SENSORE_TERRA, INPUT);*/

  scheduler.init( );

  /*scheduler.addTask(startup);
  startup.enable( );*/
  scheduler.addTask(controllo_pompa);
  //controllo_pompa.enable( ); 

  scheduler.addTask(controllo_res);
  //controllo_res.enable( );

  scheduler.addTask(stop_pompa);
  //stop_pompa.disable( );

  scheduler.addTask(controllo_atom);
  //controllo_atom.enable( );

  scheduler.addTask(controllo_led);
  //controllo_led.enable( );

  scheduler.addTask(stampa_dati);
  //stampa_dati.enable( );
 
  Serial.begin(9600);

}

void loop() {

  scheduler.execute( );

}


void accendi_pompa( ) {

  Serial.println(F("ACCENDENDO POMPA"));
  digitalWrite(RELE_POMPA, LOW);
  timer_pompa = millis( );
  stop_pompa.enable( );

}

void spegni_pompa( ) {

  if ((millis( ) - timer_pompa) >= TEMPO_EROGAZIONE) {
    Serial.println(F("SPEGNENDO POMPA"));
    /*Serial.println()*/
    digitalWrite(RELE_POMPA, HIGH);
    stop_pompa.disable( );
  }

}

void pwm_res( ) {

}

void pwm_atom( ) {

}

void pwm_led( ) {

}

void stampa( ) {

}

void spegnimento( ) {
  //...
}

void accensione( ) {

} 
