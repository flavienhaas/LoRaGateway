#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
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
          client.println("<!DOCTYPE html><html lang='fr' class=''><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1,user-scalable=no'/>");
          client.println("<style>div,fieldset,input,select{padding:5px;font-size:1em;}input{width:100%;box-sizing:border-box;}select{width:100%;}textarea{resize:none;width:98%;height:318px;padding:5px;overflow:auto;}body{text-align:center;font-family:verdana;background-color: #bcd5ff;}td{padding:0px;}button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;transition-duration:0.4s;}button:hover{background-color:#006cba;}.p{float:left;text-align:left;}.q{float:right;text-align:right;}</style>");
          client.println("<title>Passerelle LoRa</title>");
          client.println("</head>");
          client.println("<body onload='alea()'><script>function alea() {var x = document.getElementById('demo')x.innerHTML = Math.floor((Math.random() * 10) + 1);}</script>");
          client.println("<div style='text-align:left;display:inline-block;min-width:340px;'><div style='text-align:center;'><h1>Passerelle LoRa</h1></div>");
          client.println("<div><h2>[Température actuelle]</h2></div>");
          client.println("<div><p>Nb d'échant's sent = %, Echec sent : %</p></div><div id='l1' name='l1'></div><table style='width:100%'><tr><button onclick='la('?o=1');'>Informations</button></tr><tr><button onclick='alea()'>rafraichir</button></tr><tr><button onclick='la('?o=1');'>Redémarrer</button></tr></table></div>");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
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

