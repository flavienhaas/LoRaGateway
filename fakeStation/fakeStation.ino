#include <SPI.h>
#include <LoRa.h>

typedef struct
{
  uint8_t ids = 0x45;
  uint8_t idp = 0x00;
  uint16_t ts = 0x0000;
  uint16_t dt = 0x0001;
  uint16_t d1 = 0x0000;
  uint16_t d2 = 0x0000;
  uint16_t d3 = 0x0000;
}message;

message msg;

void setup() {
  SerialUSB.begin(9600);
  while (!Serial);

  SerialUSB.println("LoRa Sender");

  if (!LoRa.begin(868E6))
  {
    SerialUSB.println("Starting LoRa failed!");
    while(1);
  }
}

void loop()
{
  msg.ts = msg.ts + 1;
  msg.d1 = random(20, 40);
  msg.d2 = random(40, 60);
  msg.d3 = random(0, 100);

  SerialUSB.print("ID station : ");
  SerialUSB.print(msg.ids, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.ids, HEX);

  SerialUSB.print("ID passerelle : ");
  SerialUSB.print(msg.idp, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.idp, HEX);

  SerialUSB.print("Numero de message : ");
  SerialUSB.print(msg.ts, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.ts, HEX);

  SerialUSB.print("Type de donne : ");
  SerialUSB.print(msg.dt, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.dt, HEX);

  SerialUSB.print("Donnee 1 : ");
  SerialUSB.print(msg.d1, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.d1, HEX);

  SerialUSB.print("Donnee 2 : ");
  SerialUSB.print(msg.d2, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.d2, HEX);

  SerialUSB.print("Donnee 3 : ");
  SerialUSB.print(msg.d3, DEC);
  SerialUSB.print(", ");
  SerialUSB.println(msg.d3, HEX);
  SerialUSB.println(" ");
  
  SerialUSB.println("Sending packet !");
  LoRa.beginPacket();
  LoRa.write((uint8_t*)&msg, 12);
  LoRa.endPacket();

  SerialUSB.println("-----------------------------");
  delay(4000);
}
