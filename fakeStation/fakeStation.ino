#include <LoRa.h>
//#include <EEPROM.h>

uint16_t temp ;
uint16_t hum ;
uint16_t pluie ;
uint16_t IDSTATION;
uint16_t IDMESSAGE;

#define DelaiEntreMessages 1 // en minutes. Pour 140 messages par jour mettre 11 minutes (10,28 si c'est possible)

void setup()
{
  //Wire.begin();
  Serial.begin(9600);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) { // Démarrage du LoRa avec une fréquence de 868MHz, si il n'y a pas de retour
    Serial.println("Erreur du demarrage du LoRa!"); // on écrit un message d'erreur
    while (true);
  }
  else {
    delay(2000);
    Serial.println("RaLo OK");
  }
  IDSTATION = 1; // Lecture de l'id de la station écrit dans l'EEPROM
  //IDMESSAGE = EEPROM.read(4)*256+EEPROM.read(5); // relecture du prochain timestamp à utiliser
  //IDMESSAGE = 456; 
}

void loop() {

  static uint32_t prochainEnvoie = millis() + 0; // permet d'envoyer tout de suite
 /* static long prochainCrLf = 0;

  if ( ++prochainCrLf > 80 )
  {
    prochainCrLf = 0;
  }
*/
  if ( millis() > prochainEnvoie ) {
    prochainEnvoie = millis() + ((uint32_t)DelaiEntreMessages * (uint32_t)60 * (uint32_t)1000); // prochain envoie dans dix minutes
    Serial.print("\n\rEn procedure de travail a "); Serial.print( millis()); Serial.print(" next a "); Serial.println(prochainEnvoie);
    delay(200);

    temp = LireTemperature();
    hum = LireHumidite();
    pluie = LirePluie();
    delay(1000);
    RadioEnvoyer(IDSTATION, IDMESSAGE, temp, hum, pluie);
    delay(1000);
  }
}

