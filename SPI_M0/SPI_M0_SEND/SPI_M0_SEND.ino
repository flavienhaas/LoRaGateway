#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function

SPIClass mySPI (&sercom2, 3, 5, 4, SPI_PAD_0_SCK_3, SERCOM_RX_PAD_1); //digital 3,5,4 for PIN_SPI_MISO,  PIN_SPI_SCK,  PIN_SPI_MOSI

void setup() {
  Serial.begin(115200);

  // do this first, for Reasons
  mySPI.begin();

  // Assign pins 3, 4, 5 to SERCOM & SERCOM_ALT
  pinPeripheral(3, PIO_SERCOM_ALT);
  pinPeripheral(4, PIO_SERCOM_ALT);
  pinPeripheral(5, PIO_SERCOM);
}

uint8_t i=0;
void loop() {
  Serial.println(i);
  mySPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  mySPI.transfer(i++);
  mySPI.transfer(7);
  mySPI.endTransaction();
}
