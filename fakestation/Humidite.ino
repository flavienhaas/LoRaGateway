// renvoie le taux d'humidité en % du DHT12
uint16_t LireHumidite() {
  //float H = dht12.readHumidity(); //lecture du taux d'humidité
  float H = random(0,100); //lecture du taux d'humidité
  return (uint16_t)H; // 0% à 100%
}

