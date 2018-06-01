#include <SPI.h>                                             // to use SPI
#include <Ethernet.h>                                        // to use ethernet
#include <CProtocol12Bytes.h>                                // to use our frame protocol

byte mac[] = {0xFA, 0xE3, 0x40, 0xEF, 0xFF, 0xFD};           // set the mac address

CProtocol12Bytes trameToSend;                                // make an object with our protocol

uint16_t incrTS=0x0000;

void setup() {
  SerialUSB.begin(9600);                                     // initialise serial
  SerialUSB.println("POST");                                 // print on the screen the name of the program
  Ethernet.begin(mac);                                       // initialise ethernet
  delay(1000);
  trameToSend.codeFrame(0x05,0x07,0x0000,0x0001,0x0300,0x0005,0x0000);
}

void loop()
{
  trameToSend.setDataOne((uint16_t)(random(500, 750)));
  trameToSend.setDataTwo((uint16_t)(random(40, 60)));
  trameToSend.setDataThree((uint16_t)(random(0, 1)));
  
  EthernetClient postClient;
  String postData = "ID="+String(trameToSend.getStationId())+"&IDp="+String(trameToSend.getGatewayId())+"&TS="+String(trameToSend.getTimestampMessage())+"&DT="+String(trameToSend.getDataType())+"&D1="+String(trameToSend.getDataOne())+"&D2="+String(trameToSend.getDataTwo())+"&D3="+String(trameToSend.getDataThree());
   if (postClient.connect("btslimayrac.ovh", 80)){
     postClient.print("POST /weather/formulaire/formulaireCollecteLORA.php HTTP/1.1\n");
     postClient.print("Host: btslimayrac.ovh\n");             // specifies the Internet host and port number of the resource being requested
     postClient.print("Connection: close\n");                 // header option to signal that the connection will be closed after completion of the response
     postClient.print("Content-Type: application/x-www-form-urlencoded\n");      // values are encoded in key-value separated by '&', with a '=' between the key and the value
     postClient.print("Content-Length: ");                  // indicates the size of the entity-body, in decimal number of bytes
     postClient.print(postData.length());                  // to retrieve the size and send it
     postClient.print("\n\n");
     postClient.print(postData);                           // to send the concatenated frame
     SerialUSB.println("Post to server sent, frame :");    // to display the sent frame
     SerialUSB.println(postData);
     incrTS = trameToSend.getTimestampMessage()+1;         // to increment TS part of the frame
     trameToSend.setTimestampMessage(incrTS);
     delay(4000);
     }
    else{
      SerialUSB.println("Post failed");
    }
}
