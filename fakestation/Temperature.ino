// renvoie la temperature du DHT12 en 10iemes de degres celsius
// DHT12 va de -25 à 75° avec resolution de 0,1°
// on renvoie la (Température+40)*10
uint16_t LireTemperature() {
  //float T = dht12.readTemperature(); //lecture de la température
  float T = random(5,40); //lecture de la température
  return (uint16_t)((T + 40.0) * 10.0); // renvoie 150 -> -25°C et 1050 -> 75°C
}

