// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine

#include <SPI.h>                                             // to communicate using spi (required for our shields)
#include <LoRa.h>                                            // to use the LoRa shield
#include <CModemLoRa.h>                                      // to use personalised LoRa class
#include <CProtocol12Bytes.h>                                // to use our protocol

CModemLoRa thisLoRa;                                         // create object for personnalizeed LoRa class
CProtocol12Bytes received;                                   // create object that will be used to send data using our protocol

uint16_t saveIDandTS[255];                                   // used to send the correct frame
int numCase;

void setup(){
  SerialUSB.begin(9600);
  SerialUSB.println("LoRa frame treatment");                 // display on serial the name of the device

  thisLoRa.begin();                                          // initialise LoRa
}// end of setup

void loop() { 
  int packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&received);
    numCase = received.getStationId();
    if(received.getTimestampMessage() == saveIDandTS[numCase])
      {}
    else{
      saveIDandTS[numCase] = received.getTimestampMessage();
      //post to server
      delay(100);
      SerialUSB.print("ID = ");
      SerialUSB.print(received.getStationId(),HEX);
      SerialUSB.println(received.getGatewayId(),HEX);
      SerialUSB.print("TS = ");
      SerialUSB.println(received.getTimestampMessage(),HEX);
      SerialUSB.print("DT = ");
      SerialUSB.println(received.getDataType(),HEX);
      SerialUSB.print("D1 = ");
      SerialUSB.println(received.getDataOne(),HEX);
      SerialUSB.print("D2 = ");
      SerialUSB.println(received.getDataTwo(),HEX);
      SerialUSB.print("D3 = ");
      SerialUSB.println(received.getDataThree(),HEX);
      SerialUSB.println("NOUVELLE TRAME");
    }
  }//if (packetSize > 0)
}// end void loop
