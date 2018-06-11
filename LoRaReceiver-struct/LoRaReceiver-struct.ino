#include <SPI.h>
#include <LoRa.h>
#include <CModemLoRa.h>
#include <CProtocol12Bytes.h>

CModemLoRa thisLoRa;
CProtocol12Bytes protocol;

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.println("LoRa Receiver");
  thisLoRa.begin();
}

void loop() {
  int packetSize = thisLoRa.parsePacket();
  if (packetSize > 0)
  {
    thisLoRa.read(&protocol);                                
    SerialUSB.println("Frame received");
    delay(100);
    SerialUSB.print("ID = ");
    SerialUSB.print(protocol.getStationId(),HEX);
    SerialUSB.println(protocol.getGatewayId(),HEX);
    SerialUSB.print("TS = ");
    SerialUSB.println(protocol.getTimestampMessage(),HEX);
    SerialUSB.print("DT = ");
    SerialUSB.println(protocol.getDataType(),HEX);
    SerialUSB.print("D1 = ");
    SerialUSB.println(protocol.getDataOne(),HEX);
    SerialUSB.print("D2 = ");
    SerialUSB.println(protocol.getDataTwo(),HEX);
    SerialUSB.print("D3 = ");
    SerialUSB.println(protocol.getDataThree(),HEX);
  }
}







// objet thislora qui appele classe Lora.h et rempli la stucture de l'objet protocol, ser a allèger -5lignes
