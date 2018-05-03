#include <SPI.h>
#include <LoRa.h>
#include <String.h>
#include "Ethernet.h"

//correspond au modele de la trame
//la station est définie sur 1 octet (poid faible)
#define MAX_STATION 0x03 

#define Serial SerialUSB

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};          // set the mac address

uint16_t temp_IS=0;
uint16_t temp_H=0;
bool res;

//structure de la trame (établie en fonction de notre protocole)
typedef struct
{
  uint16_t IS;
  uint16_t TS;
  uint16_t TD;
  uint16_t D1;
  uint16_t D2;
  uint16_t D3;
}trame;

trame tab_trames[MAX_STATION]; //tableau des trames par micro-statiton

void setup() 
{
  Serial.begin(9600);
  Ethernet.begin(mac);
  Serial.println("Passerelle LoRa :");
  Serial.print("server is at ");                            // display on serial the IP you can find the webpage
  Serial.println(Ethernet.localIP());

//on démarre une instance de LoRa avec en parametre une fréquence et on bloque si on ne peux pas se connecter
  do
  {
    res = LoRa.begin(868E6);
    if(res == false)
    {
      Serial.println("Le LoRa ne peux pas établir de connexion!");
    }
  }
  while(res == false);
}

void loop() 
{
  trame traitement_trame;
  int packetSize = LoRa.parsePacket();  //On range dans cette variable la taille du packet
  uint16_t trame_recu[6];
  int id;
  int i=0;
  if (packetSize > 0) 
  {
    Serial.println("Réception des trames :");
    while (LoRa.available()) //on se met sur écoute et on lit le paquet qu'on cast en char
    {
      trame_recu[i]=LoRa.read() << 8;
      trame_recu[i] |= LoRa.read();
      Serial.print(trame_recu[i]);// read sert a lire les données du registre du LoRa
      Serial.print(".");
      i++;
    }
    
    Serial.println(" ");
    
    /*cas acceptés:
     * temp_IS != traitement_trame.IS && temp_H != traitement_trame.H
     * temp_IS != traitement_trame.IS && temp.H == traitement_trame.H
     * temp_IS == traitement_trame.IS && temp.h != traitement_trame.H
     * cas reffusé :
     * temp_IS == traitement_trame.IS && temp.h == traitement_trame.H
    */
    if(tab_trames[trame_recu[0]-1].IS == trame_recu[0] && tab_trames[trame_recu[0]-1].TS == trame_recu[1])
    {
      Serial.println("trame déja recu");
    }
    else
    {
      //on stocke les valeurs reçue de la trame dans la structure
      traitement_trame.IS=trame_recu[0];
      traitement_trame.TS=trame_recu[1];
      traitement_trame.TD=trame_recu[2];
      traitement_trame.D1=trame_recu[3];
      traitement_trame.D2=trame_recu[4];
      traitement_trame.D3=trame_recu[5];
                
      //On remplace les valeurs temp pour pouvoir les comparer avec les futures trames      
      temp_H = traitement_trame.TS;
      temp_IS = traitement_trame.IS;
      tab_trames[temp_IS-1]=traitement_trame;  

      envoyer_trame(temp_IS-1);  
    }
  }
}


/*
 * fonction envoyer par TX/RX les trames recues precedemment
 * On convertit en string le tableau de trame puis on le concatene 
 * avec les en-tetes basique du protocole HTTP
 * 
 * CODE :
 * 
 * Variables :
 * création d'une en_tete String
 * incréments
 * tab_trames
 * tableau de caractères
 * IMPOSSIBILITE : print ou write peux afficher les strings,
 * cependant on utilisera un char pour la surdéfinition de print
 * 
 * Instructions :
 * for(i=0;i<maxstation;i++) Parcourir les en_tete du tableau
 * {
 *  pour chaque en_tete, concatener les uint en string
 *  convertirle string final (1 micro-station) en char
 *  afficher le char
 *  reset du string pour pas concatener tout les caractères
 * }
 */
void envoyer_trame(uint8_t IS)
{
  String en_tete;
  char afficher_en_tete[200];
  en_tete.concat("GET /formulaireCollecteLORA.php?ID=");
  en_tete.concat(tab_trames[IS].IS);
  en_tete.concat("&IDp=");
  en_tete.concat("1");
  en_tete.concat("&TS=");
  en_tete.concat(tab_trames[IS].TS);
  en_tete.concat("&DT=");
  en_tete.concat(tab_trames[IS].TD);
  en_tete.concat("&D1=");
  en_tete.concat(tab_trames[IS].D1);
  en_tete.concat("&D2=");
  en_tete.concat(tab_trames[IS].D2);
  en_tete.concat("&D3=");
  en_tete.concat(tab_trames[IS].D3);
  en_tete.concat(" HTTP/1.0\r\nHost: btslimayrac.ovh\r\nConnection: close\r\n\f");
  
  Serial.println("On envoie une en-tete :");
  Serial.println(en_tete);
//en_tete.toCharArray(afficher_en_tete,200);
  //Serial.write(afficher_en_tete, 200);
  Serial.println(" ");
  en_tete = "";
    
}

