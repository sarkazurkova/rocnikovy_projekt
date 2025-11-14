#include <Arduino.h>
#include <IRremote.hpp>
#include <SoftwareSerial.h>
#include <RTClib.h>
#include <Wire.h>
#include <Stepper.h> 
#include "definitions.h"
#include "eeprom.h"
#include "motor.h"


bool state = LOW;
int steps = 0; // position of the door
extern Stepper stepper;

SoftwareSerial bluetooth(TX, RX);
RTC_DS3231 rtc;
DateTime time;
//------------------------------------------------SETUP
void setup() {
  pinMode(BTNOPEN, INPUT_PULLUP);
  //pinMode(BTNCLOSED, INPUT_PULLUP);
  pinMode(IRSensor, INPUT);
  pinMode(LED, OUTPUT);
  stepper.setSpeed(60);
  IrReceiver.begin(IRReceiver, ENABLE_LED_FEEDBACK);
  bluetooth.begin(9600);
  bluetooth.println("Arduino zapnuto, test Bluetooth..");
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("DS3231 nenalezen!");
    while (1);
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //staci pouze jednou, diky baterii se cas zachova
  Serial.println("Cas byl nastaven");
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");

}
//------------------------------------------------konec SETUP

//------------------------------------------------ Bluetooth funkce
void print2digits(int number) {
  if (number < 10) bluetooth.print("0");
  bluetooth.print(number);
}
// Formát pro datum
void bluetoothPrintDate(uint32_t timestamp){
  DateTime dt(timestamp);
  bluetooth.print(dt.day());
  bluetooth.print(".");
  print2digits(dt.month());
  bluetooth.print(".");
  print2digits(dt.year());
  bluetooth.print(" ");
  print2digits(dt.hour());
  bluetooth.print(":");
  print2digits(dt.minute());
  bluetooth.print(":");
  print2digits(dt.second());
}
// Výpis na bluetooth terminál
void bluetoothAllRecords() {
  bluetooth.println("\n--- Ulozene zaznamy ---");
  for (byte i = 1; i <= RECORD_COUNT; i++) {
    uint32_t ts;
    byte ev;
    readRecord(i, ts, ev);

    if (ts == 0xFFFFFFFF || ts == 0x00000000 || ev == 0xFF) continue; // prázdný slot

    bluetooth.print("#");
    bluetooth.print(i);
    bluetooth.print(" | ");
    bluetoothPrintDate(ts);
    bluetooth.print(" | Event: ");
    if(ev == 1){
      bluetooth.println("OPENED");
    }else{
    bluetooth.println("CLOSED");
    }
    if(i % 10 == 9){
      bluetooth.println("----------------------");
    }
  }
  bluetooth.println("----------------------------------------------");
}

//---------------------------------------------------Konec Bluetooth funkcí


void doorMove(unsigned long value){
  unsigned long end = 0;
  int ledTime = 0; //stav led, kazdych 5 hodnot se meni
  //Serial.println(value);
  //DateTime time  = rtc.now(); //ziska aktualni datum a cas
  switch(value){
    case OPEN: 
      logEvent(1); //zaznamená aktivitu do pameti
      //Serial.println("------");
      do{
        IrReceiver.resume(); //ceka na IR kod, pro pozastaveni
        motorStep(1); //jeden krok motoru ve smeru 1
        if(IrReceiver.decode()){
          end = IrReceiver.decodedIRData.decodedRawData;
        }
        ledState(ledTime); //zapina a vypina led
        ledTime++;
      }while(end != OPEN && end != CLOSE && !digitalRead(BTNOPEN) && digitalRead(IRSensor));
      ledTime = 0; //vynuluje hodnotu
      ledEnds(); //vypnuti led
      //timeFormat(time);
      break;

    case CLOSE:
      logEvent(2);
      //motorStart(0);
      do{
        IrReceiver.resume();
        motorStep(0);
        if(IrReceiver.decode()){
          end = IrReceiver.decodedIRData.decodedRawData;
        }
        ledState(ledTime);
        ledTime++;
      }while(end != OPEN && end != CLOSE && !digitalRead(BTNCLOSED) && digitalRead(IRSensor));
      ledEnds();
      ledTime = 0;
      break;

    case RECORD:
      printAllRecords();
      bluetoothAllRecords();
      break;
    default:
      //Serial.println(value);
      break;
  }
  //timeFormat(time);
  IrReceiver.resume();
}


//bluetooth komunikace
void bSignal(byte BluetoothData){
  switch (BluetoothData){
    case '0':
        bluetooth.println("Value 0");
        doorMove(OPEN);
        break;
      case '1':
        bluetooth.println("Value 1");
        doorMove(CLOSE);
        break;
      case '3':
        bluetoothAllRecords();
        printAllRecords();
        break;
      /*case '4':
        clearEEPROM();
        break;*/
      case '\r':
        // přesun na začátek řádku - znak CR
        break;
      case '\n':
        // odřádkování - znak LF
        break;
      default:
        // v případě přijetí ostatních znaků
        // vytiskneme informaci o neznámé zprávě
        bluetooth.println("Neznamy prikaz.");
        Serial.println(BluetoothData);
  }
}

void loop() {
 // state =0;
 if (IrReceiver.decode()){
    unsigned long receivedValue = IrReceiver.decodedIRData.decodedRawData;
    doorMove(receivedValue); 
 }
 if(bluetooth.available() > 0){
    byte BluetoothData;
    BluetoothData=bluetooth.read();
    bSignal(BluetoothData);
 }
 
}

