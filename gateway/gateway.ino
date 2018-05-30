// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine
// changed the SS port for ethernet as indicated on the README or you will not be able to use the LoRa shield as the same time as the Ethernet shield

#include <SPI.h>                                             // to communicate using spi (required for our shields)
#include <LoRa.h>                                            // to use the LoRa shield
#include <Ethernet.h>                                        // to use the ethernet shield
#include <SD.h>                                              // to use a SD card
#include <CModemLoRa.h>                                      // to use personalised LoRa class
#include <CProtocol12Bytes.h>                                // to use our protocol

#define LENMAX 80                                            // maximum size for the LoRa frame
#define Serial SerialUSB                                     // serial out on the M0 use a different function

CModemLoRa thisLoRa;                                         // create object for personnalizeed LoRa class
CProtocol12Bytes protocol;                                   // create object to store data using our protocol

File webFile;                                                // variable for the file containing the webpage

// void setSPIFrequency(uint32_t frequency);                 // set the SPI at 8MHz to use logic analyser

byte mac[] = {0xFA, 0xE3, 0x40, 0xEF, 0xFF, 0xFD};           // set the mac address
IPAddress ip(192, 1, 1, 150);                                // set the IP address for the ethernet shield, overwise the librairy use DHCP

EthernetServer server(80);                                   // initialize the EthernetServer library, using port 80 (default fot HTTP)

void setup(){
  Serial.begin(9600);
  while (!Serial);                                           // wait for serial to initialize
  Serial.println("LoRa Gateway");                            // display on serial the name of the device

  thisLoRa.begin();                                          // initialise LoRa

  //Ethernet.begin(mac, ip);                                 // initialize Ethernet shield using the set mac adress and set IP and DHCP for the rest
  Ethernet.begin(mac);                                       // initialize Ethernet shield uding the set mac and DHCP for the rest
  server.begin();                                            // initialize WebServer part of the librairy
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());                        // display on serial the IP you can find the webpage

  Serial.println("Initializing SD card...");                 // initialize SD card
  if (!SD.begin(4)) {
      Serial.println("ERROR - SD card initialization failed!");
      return;                                                // init failed
  }
  Serial.println("SUCCESS - SD card initialized.");
  if (!SD.exists("index.htm")) {                             // check for index.htm file
      Serial.println("ERROR - Can't find index.htm file!");
      return;                                                // can't find index file
  }
  Serial.println("SUCCESS - Found index.htm file.");
}                                                            // end of setup

void loop() {

//        SerialPrintElapsedTime();                          // diplay the time the frame arrived

// frame treatment
while(readFrameAndCheckTS() == true){
  // post to server
  EthernetClient postClient;
  String postData = "ID="+String(protocol.getStationId())+"&IDp="+String(protocol.getGatewayId())+"&TS="+String(protocol.getTimestampMessage())+"&DT="+String(protocol.getDataType())+"&D1="+String(protocol.getDataOne())+"&D2="+String(protocol.getDataTwo())+"&D3="+String(protocol.getDataThree());
    if (postClient.connect("btslimayrac.ovh", 80)){
      postClient.print("POST /weather/formulaire/formulaireCollecteLORA.php HTTP/1.1\n");
      postClient.print("Host: weather.btslimayrac.ovh\n");
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

// WebServer
    EthernetClient serverGateway = server.available();       // try to get client

    if (serverGateway) {                                     // got client?
        boolean currentLineIsBlank = true;
        while (serverGateway.connected()) {
            if (serverGateway.available()) {                 // client data available to read
                char c = serverGateway.read();               // read 1 byte (character) from client
                                                             // last line of client request is blank and ends with \n
                                                             // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                                                             // send a standard http response header
                    serverGateway.println("HTTP/1.1 200 OK");
                    serverGateway.println("Content-Type: text/html");
                    serverGateway.println("Connection: close");
                    serverGateway.println();
                    // send web page
                    webFile = SD.open("index.htm");          // open web page file
                    if (webFile) {                           // if the webfile exist
                        while(webFile.available()) {         // the webfile is avaible
                            serverGateway.write(webFile.read());    // send webfile to client
                        }
                        webFile.close();
                    }
                    break;
                }
                                                             // every line of text received from the client ends with \r\n
                if (c == '\n') {
                                                             // last character on line of received text
                                                             // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                                                             // a text character was received from client
                    currentLineIsBlank = false;
                }
            }                                                // end if (client.available())
        }                                                    // end while (client.connected())
        delay(1);                                            // give the web browser time to receive the data
        serverGateway.stop();                                // close the connection
    }                                                        // end if (serverGateway)
}                                                            //end void loop

bool readFrameAndCheckTS(){
  int ts1=0;                                                   // for comparing later the LoRa frames using timestamp data
  int ts2=0;                                                   // for comparing later the LoRa frames using timestamp data
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
}

//void PrintElapsedTime( boolean espaceFinal=true ){         // to display the elapsed time
//  unsigned long h,m,s = millis()/1000;
//  m=s/60;
//  h=m/60;
//  s=s-(m*60);
//  m=m-(h*60);
//  Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << (espaceFinal?" ":"");
//}
