#include <SPI.h>
#include <LoRa.h>

typedef struct{                                // frame structure
  uint16_t ID = 1025;                                       // ID
  uint16_t TS = 0;                                          // TimeStamp
  uint16_t DT = 0;                                          // Data Type
  uint16_t D1 = 0;                                          // DATA 1
  uint16_t D2 = 0;                                          // DATA 2
  uint16_t D3 = 0;                                          // DATA 3
}trame;

trame message;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  message.ID = 0x0007;
  message.TS = 0x0001;
  message.DT = 0x0001;
  message.D1 = 0x0026;
  message.D2 = 0x0003;
  message.D3 = 0x0004;

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");

  // send packet
  LoRa.beginPacket();
  LoRa.write((uint8_t)(message.ID >> 8));
  LoRa.write((uint8_t)message.ID);
  LoRa.write((uint8_t)(message.TS >> 8));
  LoRa.write((uint8_t)message.TS);
  LoRa.write((uint8_t)(message.DT >> 8));
  LoRa.write((uint8_t)message.DT);
  LoRa.write((uint8_t)(message.D1 >> 8));
  LoRa.write((uint8_t)message.D1);
  LoRa.write((uint8_t)(message.D2 >> 8));
  LoRa.write((uint8_t)message.D2);
  LoRa.write((uint8_t)(message.D3 >> 8));
  LoRa.write((uint8_t)message.D3);
  LoRa.endPacket();

  delay(3000);
}
