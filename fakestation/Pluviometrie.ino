// renvoie la pluviometrie
// 1 -> il pleut
// 0 -> pleut pas
uint16_t LirePluie() {

  const int sensorMin = 0;     // valeur capteur minimum
  const int sensorMax = 1024;  // valeur capteur maximum
  bool range;

  // lecture du capteur sur A0:
  //int sensorReading = analogRead(A0);
  int sensorReading = random(0,1024);

  Serial.print("Humidite : ");
  Serial.println(sensorReading);
  // transforme la valeur renvoyée du capteur en booléen
  range = map(sensorReading, sensorMin, sensorMax, 0, 1);

  return range;
}

