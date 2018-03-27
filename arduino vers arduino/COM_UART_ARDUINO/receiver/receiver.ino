#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

/* Use a variable called byteRead to temporarily store
   the data coming from the computer */

byte byteRead;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
   char a[8];
   /*  check if data has been sent from the computer: */
  if (Serial.available()) 
  {
    /* read the most recent byte */
    byteRead = Serial.read();
    //a[0] = atoi(byteRead);
    //a[1] = atoi(byteRead);
    Serial.print("rushin\r\n");
    /*ECHO the value that was read, back to the serial port. */
     if(byteRead == 0x61)
     {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.write(byteRead);
     }
  }

  if (mySerial.available()) 
  {
    /* read the most recent byte */
    byteRead = mySerial.read();
    //a[0] = atoi(byteRead);
  //  a[1] = atoi(byteRead);
  mySerial.print("rushin\r\n");
    /*ECHO the value that was read, back to the serial port. */
     if(byteRead == 0x61)
     {
    digitalWrite(LED_BUILTIN, HIGH);
    mySerial.write(byteRead);
     }
  }
}
