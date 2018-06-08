// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine
// changed the SS port for ethernet as indicated on the README or you will not be able to use the LoRa shield as the same time as the Ethernet shield

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
byte mac[] = { 0xFE, 0xFD, 0xBE, 0xEF, 0xFF, 0xFD };
EthernetServer server(80);                       // create a server at port 80
File webFile;                                    // var webfile de type File

void setup()
{
    SerialUSB.begin(9600);                          // for debugging
    Ethernet.begin(mac);                         // initialize Ethernet device
    server.begin();                              // start to listen for clients
    SerialUSB.print("server is at ");            // display on serial the IP you can find the webpage
    SerialUSB.println(Ethernet.localIP());
    SerialUSB.println("Initializing SD card...");
    if (!SD.begin(4)) {                                                     // initialize SD card
        SerialUSB.println("ERROR - SD card initialization failed!");
        return;                                                             // init failed
    }
    SerialUSB.println("SUCCESS - SD card initialized.");
    if (!SD.exists("index.htm")) {                                          // check for index.htm file
        SerialUSB.println("ERROR - Can't find index.htm file!");
        return;                                                             // can't find index file
    }
    SerialUSB.println("SUCCESS - Found index.htm file.");
}

void loop()
{
    EthernetClient client = server.available();                           // try to get client
    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {                                     // client data available to read
                char c = client.read();                                   // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    webFile = SD.open("index.htm");        // open web page file
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read()); // send web page to client
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
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}
