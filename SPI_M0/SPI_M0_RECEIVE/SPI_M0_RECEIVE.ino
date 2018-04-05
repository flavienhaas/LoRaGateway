#include<SPI.h>

//SPIClass maSPI (&PERIPH_SPI,  PIN_SPI_MISO,  PIN_SPI_SCK,  PIN_SPI_MOSI,  PAD_SPI_TX,  PAD_SPI_RX);


void setup() {
  // put your setup code here, to run once:
SPI.begin();
SerialUSB.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t data;
SerialUSB.println("hello2");
delay(1000);
data = SPI.transfer(0x00);
SerialUSB.println(data);
}
