#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "eeprom.h"


// Zapíše jeden byte na jednu adresu v eeprom
void writeByte(uint16_t addr, byte data) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(addr);  // jen 1 bajt adresy!
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

// Přečte jeden byte z jedné adresy
byte readByte(uint16_t addr) {
   Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(addr);  // jen 1 bajt adresy
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDR, (uint8_t)1);
  if (Wire.available()) return Wire.read();
  return 0xFF;
}

// Zaznamená datum a čas + event do 5 bytů
void saveRecord(uint8_t index, uint32_t timestamp, byte eventCode) {
  uint8_t base = index * RECORD_SIZE;
  writeByte(base + 0, (timestamp >> 24) & 0xFF);
  writeByte(base + 1, (timestamp >> 16) & 0xFF);
  writeByte(base + 2, (timestamp >> 8) & 0xFF);
  writeByte(base + 3, (timestamp >> 0) & 0xFF);
  writeByte(base + 4, eventCode);
}

// Přečte 5 bytů - jeden záznam
void readRecord(uint8_t index, uint32_t &timestamp, byte &eventCode) {
  uint8_t base = index * RECORD_SIZE;
  byte b0 = readByte(base + 0);
  byte b1 = readByte(base + 1);
  byte b2 = readByte(base + 2);
  byte b3 = readByte(base + 3);
  eventCode = readByte(base + 4);
  timestamp = ((uint32_t)b0 << 24) | ((uint32_t)b1 << 16) | ((uint32_t)b2 << 8) | b3;
}

// Uloží poslední index do prvního bytu
void saveIndex(byte index) { 
    writeByte(INDEX_ADDR, index); 
}

// Načte číslo z 1. bytu paměti, index posledního záznamu
byte loadIndex() {
  byte idx = readByte(INDEX_ADDR);
  if (idx == 0xFF) return 0; // EEPROM prázdná
  return idx;
}

// Resetuje index na 1, 0. index je pro zapisování posledního indexu 
void resetIndex() {
  saveIndex(1);  
  Serial.println("Index v EEPROM byl resetovan na 0.");
}

// Hlavní funkce pro uložení dat, posunutí indexu
void logEvent(byte eventCode) {
  byte index = loadIndex();
  DateTime now = rtc.now();
  uint32_t ts = now.unixtime();

  saveRecord(index, ts, eventCode);

  if (index < RECORD_COUNT - 1){
    index++;
    saveIndex(index);
  } else {
    resetIndex();
  }
  
  
}

// Vymaže celou EEPROM
void clearEEPROM() {
  Serial.println("Mazu EEPROM...");
  for (uint16_t i = 0; i < 256; i++) {
    writeByte(i, 0xFF); 
  }
  saveIndex(1);
  Serial.println("Hotovo. EEPROM vymazana.");
}

// Formát pro výpis času na konzoli
void printDate(DateTime now) {
  Serial.print(now.day(), DEC);
  Serial.print('.');
  Serial.print(now.month(), DEC);
  Serial.print('.');
  Serial.print(now.year(), DEC);
  Serial.print("  ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
}

// Vypíše všechny záznamy na konzoli
void printAllRecords() {
 // byte idx = loadIndex() - 10;
  Serial.println("\n--- Ulozene zaznamy ---");
  for (byte i = 1; i < RECORD_COUNT; i++) {
    uint32_t ts;
    byte ev;
    readRecord(i, ts, ev);

    if (ts == 0xFFFFFFFF || ts == 0x00000000 || ev == 0xFF) continue; // prázdný slot
    Serial.println();
    Serial.print("#");
    Serial.print(i);
    Serial.print(" | ");
    printDate(ts);
    Serial.print(" | Event: ");
    Serial.println(ev);
  }
  Serial.println("------------------------");
}
