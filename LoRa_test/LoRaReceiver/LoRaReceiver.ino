#include <SPI.h>
#include <LoRa.h>

void setup() {
  SerialUSB.begin(9600);
  while (!SerialUSB){};
  SerialUSB.println("LoRa Receiver");

  //if (!LoRa.begin(868E6)) {
  //  Serial.println("Starting LoRa failed!");
  //  while (1);
  //}
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    for (int i = 0; i < packetSize; i++)
    {
      SerialUSB.println((char)LoRa.read());
    }

    //Serial.print("with RSSi");
    //Serial.println(LoRa.packetRssi());
    delay(100);
  }
  
}
