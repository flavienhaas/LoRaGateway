#include <SPI.h>
#include <LoRa.h>

int counter = 0;
char mes[] = "message :";

void setup() {
  SerialUSB.begin(9600);
  while (!Serial);

  SerialUSB.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    SerialUSB.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  SerialUSB.print("paquet envoyé : ");
  SerialUSB.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("message : ");
  LoRa.print(counter);
  LoRa.print("\n");
  LoRa.endPacket();

  counter++;

  delay(5000);
}
