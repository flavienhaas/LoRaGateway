/*-----------------------------------
 Petite collection deviendra grande
 
  Astuces glanées ça et là
 
-----------------------------------*/


/* Permet de compacter des suites de Serial.Print
    Exemple :
    Serial.print("J'ai ");
    Serial.print(i);
    Serial.print(" bronzes de ");
    Serial.print(j);
    Serial.println(" kilo chacun."); 
    par
    Serial << "J'ai" << i << " bronzes de " << j << " kilo chacun.\n"
    Source : https://playground.arduino.cc/Main/StreamingOutput
    
    Fonctionne avec toutes les librairies d�riv�es comme LCD, SerialUSB, ..."
*/
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 


/*-----------------------------------------
 * Affichage du temps écoulé depuis le lancement
 * du programme,
 * sous le forme hh:mm:ss
 * le paramètre optionnel place un espace à la fin
 */
void SerialPrintElapsedTime( boolean espaceFinal=true ){
  unsigned long h,m,s = millis()/1000;
  m=s/60;  
  h=m/60;
  s=s-(m*60); 
  m=m-(h*60);
  Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << (espaceFinal?" ":"");
}


