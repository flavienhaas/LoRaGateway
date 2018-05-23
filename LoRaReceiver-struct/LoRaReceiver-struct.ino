#include <SPI.h>
#include <LoRa.h>

typedef struct{                                // frame structure
  uint8_t IDs = 0;                                          // station's ID
  uint8_t IDp = 0;                                          // gateway's ID
  uint16_t TS = 0;                                          // TimeStamp
  uint16_t DT = 0;                                          // Data Type
  uint16_t D1 = 0;                                          // DATA 1
  uint16_t D2 = 0;                                          // DATA 2
  uint16_t D3 = 0;                                          // DATA 3
} trame;

trame message;

uint16_t ID;

void setup() {
  SerialUSB.begin(9600);
  while (!SerialUSB){};
  SerialUSB.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0)
  {
    SerialUSB.println("Nouveau paquet");
    message.IDs = LoRa.read();
    message.IDp = LoRa.read();
    message.IDp = 0x07;
    message.TS = ((uint16_t)LoRa.read() | (LoRa.read() << 8));
    message.DT = ((uint16_t)LoRa.read() | LoRa.read() << 8);
    message.D1 = ((uint16_t)LoRa.read() | LoRa.read() << 8);
    message.D2 = ((uint16_t)LoRa.read() | LoRa.read() << 8);
    message.D3 = ((uint16_t)LoRa.read() | LoRa.read() << 8);
    ID = (uint16_t)(message.IDs | message.IDp), HEX;
    SerialUSB.println(ID, HEX);
    SerialUSB.println(message.TS, HEX);
    SerialUSB.println(message.DT, HEX);
    SerialUSB.println(message.D1, HEX);
    SerialUSB.println(message.D2, HEX);
    SerialUSB.println(message.D3, HEX);
    delay(100);
  }
}
