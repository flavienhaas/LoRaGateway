// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine
// changed the SS port for ethernet as indicated on the README or you will not be able to use the LoRa shield as the same time as the Ethernet shield

#include <SPI.h>                                             // to communicate using spi (required for our shields)
#include <LoRa.h>                                            // to use the LoRa shield
#include <Ethernet.h>                                        // to use the ethernet shield
//#include <SD.h>                                              // to use a SD card
#include <CModemLoRa.h>                                      // to use personalised LoRa class
#include <CProtocol12Bytes.h>                                // to use our protocol

CModemLoRa thisLoRa;                                         // create object for personnalizeed LoRa class
CProtocol12Bytes protocol;                                   // create object to store data using our protocol

//File webFile;                                                // variable for the file containing the webpage

String postData;
String row;

byte mac[] = {0xFA, 0xE3, 0x40, 0xEF, 0xFF, 0xFD};           // set the mac address
//IPAddress ip(192, 1, 1, 150);                                // set the IP address for the ethernet shield, overwise the librairy use DHCP

EthernetServer server(80);                                   // initialize the EthernetServer library, using port 80 (default fot HTTP)

uint16_t saveIDandTS[255];                                   // used to send the correct frame
int numCase;                                                 // used to associates timestamp and id in the arraw

// void setSPIFrequency(uint32_t frequency);                 // set the SPI at 8MHz to use logic analyser

void setup(){
  SerialUSB.begin(9600);
  delay(500);
  SerialUSB.println("LoRa Gateway");                         // display on serial the name of the device
  delay(100);
  thisLoRa.begin();                                          // initialise LoRa
  delay(500);
  //Ethernet.begin(mac, ip);                                   // initialize Ethernet shield using the set mac adress and set IP and DHCP for the rest
  Ethernet.begin(mac);                                       // initialize Ethernet shield uding the set mac and DHCP for the rest
  delay(500);
  server.begin();                                            // initialize WebServer part of the librairy
  delay(500);
  SerialUSB.print("server is at ");
  SerialUSB.println(Ethernet.localIP());                     // display on serial the IP you can find the webpage

  //SerialUSB.println("Initializing SD card...");              // initialize SD card
  //SD.begin(4);
  //delay(500);
  //SerialUSB.println("SUCCESS - SD card initialized.");
  //SD.exists("index.htm");
  //delay(500);
  //SerialUSB.println("SUCCESS - Found index.htm file.");
}// end of setup

void loop() {

//SerialPrintElapsedTime();                                    // diplay the time the frame arrived

// frame treatment
  int packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&protocol);
    numCase = protocol.getStationId();
    if(protocol.getTimestampMessage() == saveIDandTS[numCase])
      {}
    else{
      saveIDandTS[numCase] = protocol.getTimestampMessage();
      row += "<tr><td>" + String(protocol.getStationId())+"</td><td>"+String(protocol.getGatewayId())+"</td><td>"+String(protocol.getTimestampMessage())+"</td><td>" + String(protocol.getDataType())+"</td><td>" + String(protocol.getDataOne())+"</td><td>" + String(protocol.getDataTwo())+"</td><td>" + String(protocol.getDataThree())+"</td></tr>";
      //post to server
      EthernetClient postClient;
      postData = "ID="+String(protocol.getStationId())+"&IDp="+String(protocol.getGatewayId())+"&TS="+String(protocol.getTimestampMessage())+"&DT="+String(protocol.getDataType())+"&D1="+String(protocol.getDataOne())+"&D2="+String(protocol.getDataTwo())+"&D3="+String(protocol.getDataThree());
      if (postClient.connect("btslimayrac.ovh", 80)){
      postClient.print("POST /weather/formulaire/formulaireCollecteLORA.php HTTP/1.1\n");
      postClient.print("Host: btslimayrac.ovh\n");             // specifies the Internet host and port number of the resource being requested
      postClient.print("Connection: close\n");                 // header option to signal that the connection will be closed after completion of the response
      postClient.print("Content-Type: application/x-www-form-urlencoded\n");      // values are encoded in key-value separated by '&', with a '=' between the key and the value
      postClient.print("Content-Length: ");                    // indicates the size of the entity-body, in decimal number of bytes
      postClient.print(postData.length());                     // to retrieve the size and send it
      postClient.print("\n\n");
      postClient.print(postData);                              // to send the concatenated frame
      SerialUSB.println("Post to server sent");                // to display the sent frame
      SerialUSB.println(postData);
      delay(10);
      }
      else{
        SerialUSB.println("Post failed");
        SerialUSB.println(" New attempt to POST");
        if (postClient.connect("btslimayrac.ovh", 80)){
        postClient.print("POST /weather/formulaire/formulaireCollecteLORA.php HTTP/1.1\n");
        postClient.print("Host: btslimayrac.ovh\n");             // specifies the Internet host and port number of the resource being requested
        postClient.print("Connection: close\n");                 // header option to signal that the connection will be closed after completion of the response
        postClient.print("Content-Type: application/x-www-form-urlencoded\n");      // values are encoded in key-value separated by '&', with a '=' between the key and the value
        postClient.print("Content-Length: ");                    // indicates the size of the entity-body, in decimal number of bytes
        postClient.print(postData.length());                     // to retrieve the size and send it
        postClient.print("\n\n");
        postClient.print(postData);                              // to send the concatenated frame
        SerialUSB.println("Post to server sent");                // to display the sent frame
        SerialUSB.println(postData);
        delay(10);
        }
        else{
          SerialUSB.println("Post miserably failed, can't connect to btslimayrac.ovh, check your network's settings");
          }//end of secnd else
        }//end of first else
      SerialUSB.println("New frame recieved : ");
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
      SerialUSB.print("You can see the web interface at :");
      SerialUSB.println(Ethernet.localIP());
    }
  }//if (packetSize > 0)

// WebServer
    EthernetClient serverGateway = server.available();       // try to get client
    while (serverGateway) {                                     // got client?
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
                    serverGateway.println("<!DOCTYPE HTML>");
                    serverGateway.println("<html>");
                    serverGateway.print("<head>");
                    serverGateway.print("<meta http-equiv=\"refresh\" content=\"15\">");
                    serverGateway.print("<meta charset=\"utf-8\" />");
                    serverGateway.print("<link href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAQAABILAAASCwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAZWZmAHV2dgKNjo4Rent7A3JzcwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADhIrAL+/wgCnqKw3zMvMvayoqD0AAAACFRMaAMwAAAAAAAAAAAAAAAAAAAAAAAAA0stPANLLTwLTzFAi08xQPNPMUD3Sy0g6ycWOiq6+tP00gorYHHV1mLO2UDfd01AL08xQAM3ITAAAAAAA0stPANLLTgLTzFBm08xQ3tPMUPTTzFD108xQ9NTKTvZfr4X+ALjJ/zypl/3IxFDw1M1QptPMTxnTzE8AAAAAANPMUADTzFAt08xQ5dPMUP/TzFD/08xQ/9PMUP/UzE//or1g/2O6m/+sx8L/zch6/9LLTv/TzFB80cxUANnNRgDTzFAA08xQTdPMUPvTzFD/08xQ/9PMUP/TzFD/08xQ/9XMTv/RyGH/zMie/8nEbf/TzE//yslZsk+l1AoqmvkK08xQANPMUCvTzFDj08xQ/9PMUP/TzFD/08xQ/9PMUP/TzFD/0stQ/9DJUP/UzE7/1MxP/8HGYogjl/w1KJj2KtLMTwDSy04C08xQYtPMUOLTzFD/08xQ/9PMUP/TzFD/08xQ/9PMUP/UzFD/yc1Z/4TPk/8tv+SjKJf8IymZ9hsAAAAA08xPANPMUgDTzFBS08xQ9dPMUP/TzFD/08xQ/9PMUP/TzFD/1cxO/27Rpf8G1vv/Bs3+8h2p+EwnlPUgAAAAAAAAAADSy1AA0stPCtPMUHPTzFCi08xQ29PMUP/TzFD/1sxO/7/NYf8r1Nz/ANf//wbO/vIZrfhEJZP2JgAAAAAAAAAAAAAAANHKTwDRyk4At6ozANPMUD7UzE+608xQ47HJbfJH0cX/A9b+/wTS/voOwfuUL5z1Jiya9RUAAAAAAAAAAAAAAAAAAAAAAAAAANLLTwA6nuIAW6jDKXiwqkg+rthcC739rgvD/LkUufpoLJ72KTKd9jkxnfYPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGpP8AB+V+hMblP0NKJf4FiuU9TAnlPUoKZf1OyeY9RonmPYSLJv2CwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAI5b2ACSW9gkklvUTH5X2ASKW9h4ml/UGKJn1AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//8AAP4/AAD+HwAAwAcAAIADAACAAwAAgAAAAIAAAACAAAAA4AAAAOAAAADwAAAA/gAAAP4AAAD/gwAA//8AAA==\" rel=\"icon\" type=\"image/x-icon\" />");
                    serverGateway.print("</head>");
                    serverGateway.println("<style>");
                    serverGateway.println("html{font-family:\"Trebuchet MS\";text-align:center;background-color:white;color:black}");  
                    serverGateway.println("h1{text-align:center;color:red;font-size:72px}");
                    serverGateway.println("h2{text-align:center;color:red;font-size:36px}");
                    serverGateway.println("p{text-align:center;font-size:25px}");
                    serverGateway.println("table,th,td{border:2px outset black;font-size:25px}");
                    serverGateway.println("</style>");
                    serverGateway.println("<h1>IHM Web Passerelle</h1>");
                    serverGateway.println("<p>Dernière trame envoyée</p>");
                    serverGateway.println("<p>ID station : ");
                    serverGateway.print(protocol.getStationId());
                    serverGateway.print("<br />");
                    serverGateway.println("ID passerelle : ");
                    serverGateway.print(protocol.getGatewayId());
                    serverGateway.print("<br />");
                    serverGateway.println("Timestamp : ");
                    serverGateway.print(protocol.getTimestampMessage());
                    serverGateway.print("<br />");
                    serverGateway.println("Types de données : ");
                    serverGateway.print(protocol.getDataType());
                    serverGateway.print("<br />");
                    serverGateway.println("Données 1 :");
                    serverGateway.print(protocol.getDataOne());
                    serverGateway.print("<br />");
                    serverGateway.println("Données 2 :");
                    serverGateway.print(protocol.getDataTwo()); 
                    serverGateway.print("<br />");
                    serverGateway.println("Données 3 :");
                    serverGateway.print(protocol.getDataThree());
                    serverGateway.print("</p>");
                    serverGateway.print("<br/><br/>");
                    serverGateway.print("<h2>Historique des dernières trames envoyées :</h2>");
                    serverGateway.print("<table style=\"width:100%\">");
                    serverGateway.print("<tr>");
                    serverGateway.print("<th>Identifiant station</th>");
                    serverGateway.print("<th>Identifiant passerelle</th>");
                    serverGateway.print("<th>TimeStamp</th>");
                    serverGateway.print("<th>Type de données</th>");
                    serverGateway.print("<th>Donnée 1</th>");
                    serverGateway.print("<th>Donnée 2</th>");
                    serverGateway.print("<th>Donnée 3</th>");
                    serverGateway.print("</tr>");
                    serverGateway.print(row);
                    serverGateway.print("</table>");
                    serverGateway.println("</html>");
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
        break;
    }//end if (serverGateway)
}//end void loop

//void PrintElapsedTime( boolean espaceFinal=true ){         // to display the elapsed time
//  unsigned long h,m,s = millis()/1000;
//  m=s/60;
//  h=m/60;
//  s=s-(m*60);
//  m=m-(h*60);
//  Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << (espaceFinal?" ":"");
//}
