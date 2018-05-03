// envoyer les mesures
uint16_t typeDonnee = 1;    //définis le type de données envoyées

#define MSB(x)  ((uint8_t)( (x) >> 8)   ) // définition de l'octet de poids fort par un décalage de 8bits
#define LSB(x)  ((uint8_t)( (x)&0x00FF) ) // définition de l'octet de poids faible avec un masque de bits

// idStation entre 0 et 255 (le poid fort sera imposé par la passerelle)
int RadioEnvoyer( uint16_t idStation, uint16_t idMessage, uint16_t Temperature, uint16_t Humidite, uint16_t ilpleut ) {
  idMessage++;
  //EEPROM.write( 4, idMessage>>8  );
  //EEPROM.write( 5, idMessage&0x00FF  ); // ecriture du prochain timestamp à utiliser en cas de redémarrage


  Serial.print("RadioEnvoyer.idstation : ");  Serial.println(idStation);
  Serial.print("RadioEnvoyer.Temperature : "); Serial.println(Temperature);
  Serial.print("RadioEnvoyer.Humidite : ");    Serial.println(Humidite);
  Serial.print("RadioEnvoyer.Il pleut ? : ");  Serial.println(ilpleut);

  for (int i = 0; i < 3; i++) { // on envoie le message 3 fois

    LoRa.beginPacket();             //crée une en-tête automatiquement et réinitialise les registres et la taille du message

    LoRa.write( MSB(idStation) );   //écrit les données de "idStation" dans le registre définis par LoRa.beginPacket et met à jour la taille du paquet
    LoRa.write( LSB(idStation) );

    LoRa.write( MSB(idMessage) );
    LoRa.write( LSB(idMessage) );

    LoRa.write( MSB(typeDonnee) );
    LoRa.write( LSB(typeDonnee) );

    LoRa.write( MSB(Temperature) );
    LoRa.write( LSB(Temperature) );

    LoRa.write( MSB(Humidite) );
    LoRa.write( LSB(Humidite) );

    LoRa.write( MSB(ilpleut) );
    LoRa.write( LSB(ilpleut) );

    LoRa.endPacket();               //envoie les données et vide le contenu des registres

    delay(5000);
  }
}

