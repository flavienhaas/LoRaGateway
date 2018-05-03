// renvoie le taux d'humidité en % du DHT12
uint16_t LireHumidite() {
  float H = random(0,100);
  return (uint16_t)H; // 0% à 100%
}

