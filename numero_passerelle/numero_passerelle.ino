// Flavien HAAS, 2018
// before transfert, check these things:
// have all the librairies needed installed on your machine

void setup() {
Serial.begin(9600);
}
 
void loop() {
uint16_t ids = 0x00AB;
uint16_t idp = 0xFF00;
 
Serial.print(" Id station : ");
Serial.println(ids, HEX);
 
Serial.print(" Id passerelle : ");
Serial.println(idp, HEX);
 
Serial.println((uint16_t)(ids | idp), HEX);
 
Serial.println("");
delay(1000);
}
