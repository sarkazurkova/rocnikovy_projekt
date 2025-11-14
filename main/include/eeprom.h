#ifndef eeprom_h
#define eeprom_h
#include <Arduino.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include "definitions.h"

//extern SoftwareSerial bluetooth(TX, RX);
extern RTC_DS3231 rtc;

void writeByte(uint16_t addr, byte data);
byte readByte(uint16_t addr);
void saveRecord(uint8_t index, uint32_t timestamp, byte eventCode);
void readRecord(uint8_t index, uint32_t &timestamp, byte &eventCode);
void saveIndex(byte index);
void resetIndex();
void print2digits(int number);
void printDate(DateTime now);
void logEvent(byte eventCode);
void clearEEPROM();
void printAllRecords();
void bluetoothPrintDate(uint32_t timestamp);
void bluetoothAllRecords();
#endif