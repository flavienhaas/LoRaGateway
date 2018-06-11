// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine

#include <SPI.h>                                             // to communicate using spi (required for our shields)
#include <LoRa.h>                                            // to use the LoRa shield
#include <CModemLoRa.h>                                      // to use personalised LoRa class
#include <CProtocol12Bytes.h>                                // to use our protocol

CModemLoRa receiveLoRa;                                      // create object for personnalizeed LoRa class
CProtocol12Bytes goodOne;                                    // create object that will be used to send data using our protocol

void setup(){
  Serial.begin(9600);
  while (!Serial);                                           // wait for serial to initialize
  Serial.println("LoRa frame treatment");                    // display on serial the name of the device

  thisLoRa.begin();                                          // initialise LoRa
}                                                            // end of setup

void loop() { 
  // frame treatment
  while(readFrameAndCheckID() == true){
    // post to server
  }

}// end void loop

bool readFrameAndCheckID(){
  int packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&checkID);                                // objet thislora qui appele classe Lora.h et rempli la stucture de l'objet protocol, ser a allèger -5lignes
    SerialUSB.println("Frame received");
    delay(100);
  }
  id1 = checkID.getStationId();
  packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&checkID);                                // objet thislora qui appele classe Lora.h et rempli la stucture de l'objet protocol, ser a allèger -5lignes
    SerialUSB.println("Frame received");
    delay(100);
  }
  id2 = checkID.getStationId();
  if(id1==id2){
    readFrameAndCheckID();
    }
  else{
    readFrameAndCheckTS();
    return true;
    }
} //end readframeandcheckid

bool readFrameAndCheckTS(){
  int packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&protocol);                                // objet thislora qui appele classe Lora.h et rempli la stucture de l'objet protocol, ser a allèger -5lignes
    SerialUSB.println("Frame received");
    delay(100);
  }
  ts1 = protocol.getTimestampMessage();
  packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&protocol);                                // objet thislora qui appele classe Lora.h et rempli la stucture de l'objet protocol, ser a allèger -5lignes
    SerialUSB.println("Frame received");
    delay(100);
  }
  ts2 = protocol.getTimestampMessage();
  if(ts1==ts2){
      return false;
    }
  else{
    SerialUSB.println("New Frame :");
    SerialUSB.print("ID = ");
    SerialUSB.print(protocol.getStationId(),HEX);
    SerialUSB.println(protocol.getGatewayId(),HEX);
    SerialUSB.print("TS = ");
    SerialUSB.println(protocol.getTimestampMessage(),HEX);
    SerialUSB.print("DT = ");
    SerialUSB.println(protocol.getDataType(),HEX);
    SerialUSB.print("D1 = ");
    SerialUSB.println(protocol.getDataOne(),HEX);
    SerialUSB.print("D2 = ");
    SerialUSB.println(protocol.getDataTwo(),HEX);
    SerialUSB.print("D3 = ");
    SerialUSB.println(protocol.getDataThree(),HEX);
    return true;
    }
} //end readframeandcheckts


