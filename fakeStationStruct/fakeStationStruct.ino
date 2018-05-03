#include <SPI.h>
#include <LoRa.h>

#define Serial SerialUSB

typedef struct paquet_LoRa {                                       // frame structure
  uint16_t ID = 1025;                                              // ID
  uint16_t TS = 0;                                                 // TimeStamp
  uint16_t DT = 0;                                                 // Data Type
  uint16_t D1 = 0;                                                 // DATA 1
  uint16_t D2 = 0;                                                 // DATA 2
  uint16_t D3 = 0;                                                 // DATA 3
} trame;

trame message;

void setup(){
  Serial.begin(9600);
  while (!Serial);
  Serial.println("fakeLoRastation");
  if( !LoRa.begin(868E6) ){ 
    Serial.print("Echec de l'initialisation LoRa !\n");
    while(true);
  }
}//setup()

void loop() {
  int c=0;
  message.D1=random(0,65535);
  message.D2=random(0,65535);
  message.D3=random(0,65535);

  String strID =  String(message.ID);//0x00
  String strTS =  String(message.TS);//0x0000
  String strDT =  String(message.DT);//0x0000
  String strD1 =  String(message.D1);//0x0000
  String strD2 =  String(message.D3);//0x0000
  String strD3 =  String(message.D3);//0x0000

  for(c=0;c<3;c++){
  LoRa.beginPacket(false);
  LoRa.write( (uint8_t*)&message, sizeof(message));
  LoRa.endPacket();
  Serial.println("\nPacquet envoyé: ");
  Serial.println("ID Passerelle et station envoyés : "+strID);
  Serial.println("Timestamp envoyée: "+strTS);
  Serial.println("Type de données envoyé: "+strDT);
  Serial.println("Champ de données 1 envoyé: "+strD1);
  Serial.println("Champ de données 2 envoyé: "+strD2);
  Serial.println("Champ de données 3 envoyé: "+strD3);

  delay(1000);
  }//findufor
  
  message.TS = message.TS + 1; //timestamp

  delay(3000);
  
}//loop()
