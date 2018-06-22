#include <SPI.h>
#include <LoRa.h>
#include <CModemLoRa.h>
#include <CProtocol12Bytes.h>

//#define Serial SerialUSB

CProtocol12Bytes protocol;
CModemLoRa thisLoRa;

uint16_t incrTS=0x0000;

void setup() {
  Serial.begin(9600);
  Serial.println("LoRa Sender");
  
  thisLoRa.begin();
  protocol.codeFrame(0x0A,0x00,0x0000,0x00FF,0x5BA2,0x0D4C,0x929A); // Name : LORA-TEST1 or 0x929A for *2

  LoRa.beginPacket();
  LoRa.write((uint8_t*)&protocol, 12); // Send name
  LoRa.endPacket();
}

void loop()
{
  protocol.setDataOne((uint16_t)(random(500, 750)));
  protocol.setDataTwo((uint16_t)(random(40, 60)));
  protocol.setDataThree((uint16_t)(random(0, 2)));
  incrTS = protocol.getTimestampMessage()+1;
  protocol.setTimestampMessage(incrTS);

  Serial.print("ID = ");
  Serial.print(protocol.getStationId(),HEX);
  Serial.println(protocol.getGatewayId(),HEX);
  Serial.print("TS = ");
  Serial.println(protocol.getTimestampMessage(),HEX);
  Serial.print("DT = ");
  Serial.println(protocol.getDataType(),HEX);
  Serial.print("D1 = ");
  Serial.println(protocol.getDataOne(),HEX);
  Serial.print("D2 = ");
  Serial.println(protocol.getDataTwo(),HEX);
  Serial.print("D3 = ");
  Serial.println(protocol.getDataThree(),HEX);

  for(int i=0; i<3; i++){
      Serial.println("Frame sent !");
      LoRa.beginPacket();
      LoRa.write((uint8_t*)&protocol, 12);
      LoRa.endPacket();
    
      Serial.println("-----------------------------");
      delay(4000);
    }
    delay(10000);
}
