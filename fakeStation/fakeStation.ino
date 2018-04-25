#include <SPI.h>
#include <LoRa.h> // si ce n'est déjà fait, importez la librairie LoRa via le gestionnaire de bibliothèque

struct message {                                            // frame structure
  uint16_t ID;                                              // ID
  uint16_t TS;                                              // TimeStamp
  uint16_t DT;                                              // Data Type
  uint16_t D1;                                              // DATA 1
  uint16_t D2;                                              // DATA 2
  uint16_t D3;                                              // DATA 3
};

void setup(){
  Serial.begin(9600);
  while (!Serial);
  Serial.println("fakeLoRastation");
 // LoRa.setSPIFrequency(4E6); //défaut 8MHz trop rapide pour l'analyseur
  if( !LoRa.begin(868E6) ){ 
    Serial.print("Echec de l'initialisation LoRa !\n");
    while(true);
  }
}//setup()

void loop() {
  static byte N=0;
  static byte 
  message message = {0};

  Serial.print("Envoi du message \"N="); Serial.print(N); Serial.println("\"");
  message.ID = 0x30; // n° d'identification de l'émetteur
  message.TS = 0x00;
  message.DT = 0x00;
  message.D1 = '0'+ (N/100)%10;
  message.D2 = '0'+ (N/10 )%10;
  message.D3 = '0'+ (N/1  )%10;
  
  LoRa.beginPacket(false);
  LoRa.write( (uint8_t*)&message, sizeof(message));
  LoRa.endPacket();
  delay(5000);
}//loop()
