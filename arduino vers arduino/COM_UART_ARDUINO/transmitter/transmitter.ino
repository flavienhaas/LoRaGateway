/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 7;

void setup() {
  Serial.begin(9600);
  pinMode(pushButton, INPUT);
}

void loop() {
  int buttonState = digitalRead(pushButton);
  //if(buttonState == 1)
  //{
  Serial.println("a");
  //}
  delay(1000);
}
