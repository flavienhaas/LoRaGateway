// Flavien HAAS, 2018
// before transfert, check that you have changed the SS port as indicated on the README or you will not be able to use the LoRa shield as the same time as the Ethernet shield

#include <SPI.h>                                            // to communicate using spi (required for our shields)
#include <LoRa.h>                                           // to use the LoRa shield
#include <Ethernet.h>                                       // to use the ethernet shield
#include <SD.h>                                             // to use a SD card
#include <CModemLoRa.h>                                     // to use personalised LoRa class
#include <CProtocol12Bytes.h>                               // to use our protocol


#define LENMAX 80                                           // maximum size for the LoRa frame
#define Serial SerialUSB                                    // serial out on the M0 use a different function

CModemLoRa thisLoRa;                                        // create object for personnalizeed LoRa class
CProtocol12Bytes protocol;                                  // create object to store data using our protocol

// void setSPIFrequency(uint32_t frequency);                // set the SPI at 8MHz to use logic analyser

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};          // set the mac address

//IPAddress ip(10, 0, 0, 49);                               // set the IP address for the ethernet shield, overwise the librairy use DHCP

EthernetServer server(80);                                  // initialize the EthernetServer library, using port 80 (default fot HTTP)

void setup(){
  Serial.begin(9600);
  while (!Serial);                                          // wait for serial to initialize
  Serial.print("Passerelle LoRa\n");                        // display on serial the name of the device

  if( !LoRa.begin(868E6) ){                                 // initialise LoRa and display a message if an error occur
    Serial.print("Echec de l'initialisation LoRa !\n");
    while(true);}

  //Ethernet.begin(mac, ip);                                // initialize Ethernet shield using the set mac adress and set IP
  Ethernet.begin(mac);                                      // initialize Ethernet shield uding the set mac and DHCP for the IP
  server.begin();                                           // initialize WebServer part of the librairy
  Serial.print("server is at ");                            // display on serial the IP you can find the webpage
  Serial.println(Ethernet.localIP());
}

//void SerialPrintElapsedTime( boolean espaceFinal=true ){  // to display the elapsed time
//  unsigned long h,m,s = millis()/1000;
//  m=s/60;
//  h=m/60;
//  s=s-(m*60);
//  m=m-(h*60);
//  Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << (espaceFinal?" ":"");
//}

void loop() {
// LoRa receiver
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0)
  {
    SerialUSB.println("Nouveau paquet");

    thisLoRa.read(&protocol);                               // objet thislora qui appele classe Lora.h et rempli la stucture de l'objet protocol, ser a allèger -5lignes
    
    SerialUSB.println(protocol.getStationId(),HEX);
    SerialUSB.println(protocol.getGatewayId(),HEX);
    SerialUSB.println(protocol.getTimestampMessage(),HEX);
    SerialUSB.println(protocol.getDataType(),HEX);
    SerialUSB.println(protocol.getDataOne(),HEX);
    SerialUSB.println(protocol.getDataTwo(),HEX);
    SerialUSB.println(protocol.getDataThree(),HEX);
    delay(100);
  }

//        SerialPrintElapsedTime();                         // diplay the time the frame arrived

// post to server
  EthernetClient client;
  String postdata="&ID="+String(protocol.getStationId())+"&IDp="+String(protocol.getGatewayId())+"&TS="+String(protocol.getTimestampMessage())+"&DT="+String(protocol.getDataType())+"&D1="+String(protocol.getDataOne())+"&D2="+String(protocol.getDataTwo())+"&D3="+String(protocol.getDataThree());
  bool connected = client.connect(server, 80);
    if (connected){
      client.println("POST /formulaireCollecte.html HTTP/1.1");
      client.println("Host: btslimayrac.ovh");
      client.println("Cache-Control: no-cache");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println(postData);
      }

// WebServer
    EthernetClient serverGateway = server.available();      // try to get client

    if (serverGateway) {                                    // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {                       // client data available to read
                char c = client.read();                     // read 1 byte (character) from client
                                                            // last line of client request is blank and ends with \n
                                                            // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                                                            // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    webFile = SD.open("index.html");         // open web page file
                    if (webFile) {                           // if the webfile exist
                        while(webFile.available()) {         // the webfile is avaible
                            client.write(webFile.read());    // send webfile to client
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
        } // end while (client.connected())
        delay(1);                                            // give the web browser time to receive the data
        client.stop();                                       // close the connection
    }                                                        // end if (serverGateway)
}                                                            //end void loop
