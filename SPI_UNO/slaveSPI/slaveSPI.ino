/*
 * Pins: -> D10 : SS (Slave Select)
 *       -> D11 : MOSI (Master Out Slave In) (Communication maitre -> esclave)
 *       -> D12 : MISO (Master In Slave Out) (Communication esclave -> maitre)
 *       -> D13 : SCLK (Serial Clock)
 * Informations : -> Si la pin SS n'est pas configuré en mode OUTPUT, le périphérique
 *                   sera considéré comme un esclave.
 *                -> Ici, pas besoin de définir une pin SS car c'est un esclave
 * 
  */

// Inclusion des bibliothèques
#include <SPI.h>

//Variables
const int MOSI_PIN = 11;
const int MISO_PIN = 12;
const int SCLK_PIN = 13;

void setup()
{
  // Configuration des pins du SPI
  pinMode(MOSI_PIN,INPUT);
  pinMode(MISO_PIN,OUTPUT);
  pinMode(SCLK_PIN,INPUT);

  // Initialisation du port série
  Serial.begin(9600);

  // Initialisation du bus SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4); // Diviseur d'horloge du bus SPI (choisir la fréquence de l'horloge en quart de l'horloge du microcontroleur)
  //SPI.setBitOrder(MSBFIRST); // Sens d'envoi des bits (most significant bit first)
  //SPI.setDataMode(SPI_MODE0); // Mode de transmission des données (parité ety phase de l'horloge)
}

void loop()
{
  // Transmettre des informations
  byte incomingByte = 0x0;
  incomingByte = SPI.transfer(0x0); // se qu'on recois = transfert(ce qu'on transmet)
  Serial.println(incomingByte,HEX);

  delay(1000);
}

