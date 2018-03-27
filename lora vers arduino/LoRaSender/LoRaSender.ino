#include <SPI.h>
#include <LoRa.h>

int counter = 0;
char mes[] = "message :";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("paquet envoyé : ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("message : ");
  LoRa.print(counter);
  LoRa.print("\n");
  LoRa.endPacket();

  counter++;

  delay(1000);
}
