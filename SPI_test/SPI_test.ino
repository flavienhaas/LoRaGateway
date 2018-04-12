#include <SPI.h>
#define ss 10

void setup() {
  SerialUSB.begin(9600);
  pinMode(ss, OUTPUT);
  SPI.begin(); //initiate the SPI communicaton
  SPI.setBitOrder(MSBFIRST);
}

void loop() {
  digitalWrite(ss, LOW);
  SPI.transfer(0x02);
  digitalWrite(ss, HIGH);
  SerialUSB.println("0x02 sent");
  SPI.endTransaction();
  delay(1000);
}
