// Flavien HAAS, 2018
// before transfert, check that you have changed the SS port as indicated on the README or you will not be able to use the LoRa shield as the same time as the Ethernet shield

#include <SPI.h>                                            // to communicate using spi (required for our shields)
#include <LoRa.h>                                           // to use the LoRa shield
#include "Ethernet.h"                                       // to use the ethernet shield

#define LENMAX 80                                           // maximum size for the LoRa frame
#define Serial SerialUSB                                    // serial out on the M0 use a different function

// void setSPIFrequency(uint32_t frequency);                // set the SPI at 8MHz to use logic analyser

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};          // set the mac address

//IPAddress ip(10, 0, 0, 49);                               // set the IP address for the ethernet shield, overwise the librairy use DHCP

EthernetServer server(80);                                  // initialize the EthernetServer library, using port 80 (default fot HTTP)

struct message {                                            // frame structure
  uint16_t ID;                                              // ID
  uint16_t TS;                                              // TimeStamp
  uint16_t DT;                                              // Data Type
  uint16_t D1;                                              // DATA 1
  uint16_t D2;                                              // DATA 2
  uint16_t D3;                                              // DATA 3
};

void setup(){
  Serial.begin(9600);
  while (!Serial);                                          // wait for serial to initialize
  Serial.print("Passerelle LoRa\n");                        // display on serial the name of the device

  if( !LoRa.begin(868E6) ){
    Serial.print("Echec de l'initialisation LoRa !\n");
    while(true);}                                           // initialize LoRa shield LoRa at 868 MHz

  //Ethernet.begin(mac, ip);                                // initialize Ethernet shield using the set mac adress and set IP
  Ethernet.begin(mac);                                      // initialize Ethernet shield uding the set mac and DHCP for the IP
  server.begin();                                           // initialize WebServer part of the librairy
  Serial.print("server is at ");                            // display on serial the IP you can find the webpage
  Serial.println(Ethernet.localIP());
}

//void SerialPrintElapsedTime( boolean espaceFinal=true ){    // to display the elapsed time
//  unsigned long h,m,s = millis()/1000;
//  m=s/60;
//  h=m/60;
//  s=s-(m*60);
//  m=m-(h*60);
//  Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << (espaceFinal?" ":"");
//}

void loop() {
// LoRa receiver
    static byte tampon[LENMAX]={0};                         // if the module receive a frame, it willnot be null
    int longueurTrame;
    longueurTrame=LoRa.parsePacket();
    if( longueurTrame > 0 ){
        if( longueurTrame>LENMAX ){                         // copy of the frame to cache (LENMAX) and verify if the frame is to big
            Serial.print("Trame reçue trop grande : ");
            Serial.println(longueurTrame);
            longueurTrame=LENMAX;                           // cut the frame to LENMAX size
        }
        for( int i=0; i<longueurTrame; i++ ){
            tampon[i]=(byte)LoRa.read();
        }
//        SerialPrintElapsedTime();                           // diplay the time the frame arrived
        Serial.print("0x");
        for( int i=0; i<longueurTrame; i++ ){               // display the frame in hexadecimal
            if( tampon[i] < 0x0F ) Serial.print("0");
            Serial.print( tampon[i], HEX );
        }
        Serial.print( " " );
        for( int i=0; i<longueurTrame; i++ ){
            if( (tampon[i] < 0x20)||(tampon[i] > 0x7E) ){
                Serial.print( ".");                         // this character isn't printable (displayable)
            }else{
                Serial.print( (char)tampon[i] );            // display the frame in ASCII
            }
        }
        Serial.print( "\n" );
    }                                                       // end of if LoRa.parsePacket
    delay(10);
// WebServer
  EthernetClient client = server.available();               // WebServer :listen for incoming clients
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;                      // an http request ends with a blank line
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {              // send the beginning of a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");              // the connection will be closed after completion of the response
          client.println("Refresh: 5");                     // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {                                    // send a new blank line to indicate the end of the connection
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);                                               // give the web browser time to receive the data
    client.stop();                                          // close the connection of the webserver
  }
}
