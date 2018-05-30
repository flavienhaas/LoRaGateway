#include <SPI.h>
#include <Ethernet.h>
#include <CProtocol12Bytes.h>                                // to use our protocol
 
byte mac[] = {0xFA, 0xE3, 0x40, 0xEF, 0xFF, 0xFD};           // set the mac address
EthernetClient client;

CProtocol12Bytes trameToSend;

uint16_t incrTS=0x0000;

void setup(){
  SerialUSB.begin(9600);
  Ethernet.begin(mac);
  delay(1000);
  Serial.println("connecting...");

  trameToSend.setStationId(0x07);
  trameToSend.setGatewayId(0x07);
  trameToSend.setTimestampMessage(0x0000);
  trameToSend.setDataType(0x0001);
  trameToSend.setDataOne(0x0004);
  trameToSend.setDataTwo(0x0005);
  trameToSend.setDataThree(0x0006);
  
}
 
void loop(){
  EthernetClient postClient;
  String postData = "ID="+String(trameToSend.getStationId())+"&IDp="+String(trameToSend.getGatewayId())+"&TS="+String(trameToSend.getTimestampMessage())+"&DT="+String(trameToSend.getDataType())+"&D1="+String(trameToSend.getDataOne())+"&D2="+String(trameToSend.getDataTwo())+"&D3="+String(trameToSend.getDataThree());
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
      incrTS = trameToSend.getTimestampMessage()+1,
      trameToSend.setTimestampMessage(incrTS);
      }
     else{
      Serial.println("Post failed");
     }
 
}
