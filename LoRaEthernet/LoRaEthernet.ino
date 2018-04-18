#include <SPI.h>
#include <LoRa.h>
#include "Ethernet.h"
#include "util.h" // pour avoir l'affichage du temps écoulé

#define LENMAX 80 // taille maximale acceptée pour la trame
#define Serial SerialUSB

// void setSPIFrequency(uint32_t frequency); // 8MHz par défaut c'est bq pour l'analyseur logique

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 0, 0, 49);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup(){
  Serial.begin(9600);
  while (!Serial);
  Serial.print("Recepteur LoRa\n");
  // initialiser le shield LoRa en 868 MHz
  if( !LoRa.begin(868E6) ){
    Serial.print("Echec de l'initialisation LoRa !\n");
    while(true); // on se bloque ici et on ne va pas plus loin
  }
  Ethernet.begin(mac, ip);
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



