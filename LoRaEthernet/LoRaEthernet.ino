// Flavien HAAS, 2018
// to compile as on the diagrams, check that you have changed the SS port as indicated on the README

#include <SPI.h>              // to communicate using spi (required for our shields)
#include <LoRa.h>             // to use the LoRa shield
#include "Ethernet.h"         // to use the ethernet shield
#include "util.h"             // to have the display of the elapsed time

#define LENMAX 80             // maximum size for the LoRa frame
#define Serial SerialUSB      // serial out on the M0 use a different function

// void setSPIFrequency(uint32_t frequency);            // set the SPI at 8MHz to use logic analyser

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      // set the mac address

//IPAddress ip(10, 0, 0, 49);                           //set the IP address for the ethernet shield, overwise the librairy use DHCP

EthernetServer server(80);                              // initialize the EthernetServer library, using port 80 (default fot HTTP)

void setup(){
  Serial.begin(9600);
  while (!Serial);                                      // wait for serial to initialize
  Serial.print("Passerelle LoRa\n");                    // display on serial the name of the device

  if( !LoRa.begin(868E6) ){
    Serial.print("Echec de l'initialisation LoRa !\n");
    while(true);}                                       // initialize LoRa shield LoRa at 868 MHz

  //Ethernet.begin(mac, ip);                            // initialize Ethernet shield using the set mac adress and set IP
  Ethernet.begin(mac);                                  // initialize Ethernet shield uding the set mac and DHCP for the IP
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
    static byte tampon[LENMAX]={0};
    int longueurTrame;
    // si le module a reçu une trame alors sa longueur sera non nulle
    longueurTrame=LoRa.parsePacket();
    if( longueurTrame > 0 ){
        //---- copie de la trame depuis le modem vers le tampon ----
        if( longueurTrame>LENMAX ){
            Serial.print("Trame reçue est trop grande pour le tampon : ");
            Serial.println(longueurTrame);
            longueurTrame=LENMAX; // troncature
        }
        for( int i=0; i<longueurTrame; i++ ){
            tampon[i]=(byte)LoRa.read();
        }
        //---- affichage de l'heure d'arrivée ----
        SerialPrintElapsedTime();
        //---- affichage en hexadécimal ----
        Serial.print("0x");
        for( int i=0; i<longueurTrame; i++ ){
            if( tampon[i] < 0x0F ) Serial.print("0");
            Serial.print( tampon[i], HEX );
        }
        //---- affichage en ASCII ----
        Serial.print( " " );
        for( int i=0; i<longueurTrame; i++ ){
            if( (tampon[i] < 0x20)||(tampon[i] > 0x7E) ){
                Serial.print( "."); // ce caractère est non imprimable
            }else{
                Serial.print( (char)tampon[i] );
            }
        }
        Serial.print( "\n" );
    }//if LoRa.parsePacket
    delay(10);
      // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
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
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
