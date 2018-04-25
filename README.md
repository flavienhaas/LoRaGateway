# passerelleLoRa
Projet BTS SNIR 2ème année
Gateway between a weather MicroStation using LoRa modulation and a web server. This depo is segmented in different programs for arduino using ATMEGA328P and SAMD21.

#Attention !#
Because we're using two arduino shields using SPI communication, we need to select a chip select for each shield. I decided to use D11 for the Ethernet shield (10 for the LoRa, 4 for the SD).

To do this, edit w5100.h in the default ethernet librairy in the installation folder of your arduino IDE (on windows, in adminisstrator mode ‪C:\Program Files (x86)\Arduino\libraries\Ethernet\src\utility\w5100.h).
