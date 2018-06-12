#include <SPI.h>
#include <LoRa.h>
#include <CModemLoRa.h>
#include <CProtocol12Bytes.h>

CProtocol12Bytes protocol;
CModemLoRa thisLoRa;

uint16_t incrTS=0x0000;

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.println("LoRa Sender");
  thisLoRa.begin();
  protocol.codeFrame(0x0B,0x00,0x0000,0x0001,0x0000,0x0000,0x0000);
}

void loop()
{
  protocol.setDataOne((uint16_t)(random(500, 750)));
  protocol.setDataTwo((uint16_t)(random(40, 60)));
  protocol.setDataThree((uint16_t)(random(0, 1)));
  incrTS = protocol.getTimestampMessage()+1;
  protocol.setTimestampMessage(incrTS);

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

  for(int i=0; i<3; i++){
      SerialUSB.println("Frame sent !");
      LoRa.beginPacket();
      LoRa.write((uint8_t*)&protocol, 12);
      LoRa.endPacket();
    
      SerialUSB.println("-----------------------------");
      delay(4000);
    }
    delay(10000);
}
