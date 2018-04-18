
#ifndef DaKTASK
#define DaKTASK

// la tàche est effectuee avec un delai de PeriodeMilliSecondes entre deux executions. 
// consomme 4 octets de RAM
// exemple : cron(1000){ // code � r�p�ter toutes les secondes }
#define cron(PeriodeMilliSecondes)  for( \
    static unsigned long __nextmillis = 0; \
    millis() - __nextmillis >= (unsigned long)(PeriodeMilliSecondes); \
    __nextmillis = millis() \
    )

	// la tàche est effectuee avec un delai de PeriodeMilliSecondes entre deux executions. 
// consomme 4 octets de RAM
#define task(NomTache,PeriodeMilliSecondes)  for( \
    static unsigned long DaKTASK_ ## NomTache = 0; \
    millis() - DaKTASK_ ## NomTache >= (unsigned long)(PeriodeMilliSecondes); \
    DaKTASK_ ## NomTache = millis() \
    )

// la tàche est effectuee pr�cis�ment toutes les PeriodeMilliSecondes. 
// consomme 8 octets de RAM
#define ftask(NomTache,PeriodeMilliSecondes)  for( \
    static unsigned long DaKTASK_ ## NomTache = 0, millis_ ## NomTache = 0; \
    (millis_ ## NomTache = millis()) - DaKTASK_ ## NomTache >= (unsigned long)(PeriodeMilliSecondes); \
    DaKTASK_ ## NomTache = millis_ ## NomTache \
    )
	

#endif



/*

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  Task( Toto, 1500 ){
      Serial.print("Toto=");
      Serial.println( millis() );
  }
  
  Task( line, 3000) Serial.println(F("-----------------"));
  
  FTask( LED, 100 ){ PORTB ^= 0x20; }  // clignoter la LED  
  
  cron(500){ PORTB ^= 0x20; }  // clignoter la LED toutes les secondes
  
  delay(11);
}
*/
