#include <Arduino.h>
#include <Wire.h>



#define EEPROM_ADDR 0x50 //adresa eeprom

void writeByte(uint16_t addr, byte data) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(addr);  // jen 1 bajt adresy!
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

byte readByte(uint16_t addr) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(addr);  // jen 1 bajt adresy
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDR, (uint8_t)1);
  if (Wire.available()) return Wire.read();
  return 0xFF;
}

void setup() {
  Serial.begin(9600);
  //int count=0;
  Wire.begin();
  Serial.begin(9600);
  Serial.println("EEPROM test start...");

  //writeByte(0, 42);        // zapíšeme 42 na adresu 0
  delay(10);               // jistota

  byte val = readByte(0);  // přečteme zpět
  Serial.print("Precteno z EEPROM: ");
  Serial.println(val);
    
}

void loop() {
  int sRead;
  if(Serial.available()){
    sRead = Serial.read();
    if(sRead == '1'){
      for(int i = 0; i< 10; i++) {
    Serial.print(readByte(i), HEX); 
    Serial.print(".");
    }
    Serial.println();
    }else{
      //Serial.println(sRead);
    }
  }
}

