// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine

#include <SPI.h>                                             // to communicate using spi (required for our shields)
#include <LoRa.h>                                            // to use the LoRa shield
#include <CModemLoRa.h>                                      // to use personalised LoRa class
#include <CProtocol12Bytes.h>                                // to use our protocol

CModemLoRa thisLoRa;                                         // create object for personnalizeed LoRa class
CProtocol12Bytes goodOne;                                   // create object to store data using our protocol

int ts1=0;                                                   // for comparing later the LoRa frames using timestamp data
int ts2=0;                                                   // for comparing later the LoRa frames using timestamp data
int id1=0;                                                   // for comparing later the LoRa frames using ID data
int id2=0;                                                   // for comparing later the LoRa frames using ID data

bool readFrameAndCheckID();                                  // function that compares station's ID
bool readFrameAndCheckTS();                                  // function that compares frame's TimeStamp

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
    EthernetClient postClient;
    String postData = "ID="+String(goodOne.getStationId())+"&IDp="+String(goodOne.getGatewayId())+"&TS="+String(goodOne.getTimestampMessage())+"&DT="+String(goodOne.getDataType())+"&D1="+String(goodOne.getDataOne())+"&D2="+String(goodOne.getDataTwo())+"&D3="+String(goodOne.getDataThree());
      if (postClient.connect("btslimayrac.ovh", 80)){
        postClient.print("POST /weather/formulaire/formulaireCollecteLORA.php HTTP/1.1\n");
        postClient.print("Host: btslimayrac.ovh\n");
        postClient.print("Connection: close\n");
        postClient.print("Content-Type: application/x-www-form-urlencoded\n");
        postClient.print("Content-Length: ");
        postClient.print(postData.length());
        postClient.print("\n\n");
        postClient.print(postData);
        Serial.println("Post to server sent");
        }
       else{
        Serial.println("Post failed");
       }
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


