#include <Arduino.h>
#include <IRremote.hpp>
#include <SoftwareSerial.h>
#include <RTClib.h>
#include <Wire.h>


#define IRReceiver 3
#define IRSensor 2
#define BTNOPEN 8
#define LED 9
#define TX 10
#define RX 11
//#define BTNCLOSED 13
#define EEPROM_ADDR 0x50 //adresa eeprom
#define RECORD_SIZE 5 //velikost zaznamu v bytech
#define RECORD_COUNT 10 //pocet zaznamu, po prekroceni 10 se nejstarsi prepise
#define INDEX_ADDR 0 // adresa indexu ig
#define DATA_START 1 // prvni byte pro data

bool state = LOW;
int steps; // position of the door

#define ROUNDS 20
#define OPEN 2991456000
#define CLOSE 2874474240
#define RECORD 3008167680
#define MAXSTEPS 300

SoftwareSerial bluetooth(TX, RX);
RTC_DS3231 rtc;
DateTime time;

void setup() {
  pinMode(BTNOPEN, INPUT_PULLUP);
  //pinMode(BTNCLOSED, INPUT_PULLUP);
  pinMode(IRSensor, INPUT);
  pinMode(LED, OUTPUT);
  IrReceiver.begin(IRReceiver, ENABLE_LED_FEEDBACK);
  bluetooth.begin(9600);
  bluetooth.println("Arduino zapnuto, test Bluetooth..");
  Serial.begin(9600);
  //Wire.begin();
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


void eepromWriteByte(uint16_t addr, byte data) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(addr);  // jen 1 bajt adresy!
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

byte eepromReadByte(uint16_t addr) {
   Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(addr);  // jen 1 bajt adresy
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDR, (uint8_t)1);
  if (Wire.available()) return Wire.read();
  return 0xFF;
}

void timeFormat(DateTime now){
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
  Serial.println(now.second(), DEC);
}

void motorStep(bool dir){
    Serial.print("+ STEP TO ");
    if(dir){
      Serial.println("RIGHT!");
      steps++;
    }else{
      Serial.println("LEFT!");
      steps--;
    }
}

void motorStart(bool dir){
  //motor speed to lower value here
  state = !state;
  digitalWrite(LED, state);
  Serial.print("stav led: ");
  Serial.println(state);
  for(int i = 0; i < ROUNDS; ++i){
    Serial.print(i);
    Serial.print(". STEP TO ");
    if(dir){
      Serial.println("RIGHT!");
    }else{
      Serial.println("LEFT!");
    }
  }
  //back to normal speed
}

void ledState(int ledTime){
  if(ledTime%20==0){
      state = !state;
      digitalWrite(LED, state);
  }
}

void ledEnds(){
  state = LOW;
  digitalWrite(LED, state);
}

void doorMove(unsigned long value){
  unsigned long end = 0;
  int ledTime = 0;
  //Serial.println(value);
  time = rtc.now();
  switch(value){
    case OPEN: 
      motorStart(1);
      //Serial.println("------");
      do{
        IrReceiver.resume();
        motorStep(1);
        if(IrReceiver.decode()){
          end = IrReceiver.decodedIRData.decodedRawData;
        }
        ledState(ledTime);
        ledTime++;
      }while(end != OPEN && end != CLOSE && digitalRead(BTNOPEN) && digitalRead(IRSensor));
      ledTime = 0;
      ledEnds();
      timeFormat(time);
      eepromWriteByte(0, 42);
      break;
    case CLOSE:
      motorStart(0);
      do{
        IrReceiver.resume();
        motorStep(0);
        if(IrReceiver.decode()){
          end = IrReceiver.decodedIRData.decodedRawData;
        }
        ledState(ledTime);
        ledTime++;
      }while(end != OPEN && end != CLOSE && digitalRead(BTNOPEN) && digitalRead(IRSensor));
      ledEnds();
      timeFormat(time);
      ledTime = 0;
      
      eepromWriteByte(0, 22);
      break;
    case RECORD:
      Serial.print("Hodnota na indexu 0: ");
      byte num = eepromReadByte(0);
      Serial.println(num);
      break;
    default:
      Serial.println(value);
      break;
  }
  //timeFormat(time);
  IrReceiver.resume();
}


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

void doorPosition(){
  if(digitalRead(BTNOPEN)){
    steps = 0;
  }
  if(digitalRead(BTNOPEN)){
    steps = MAXSTEPS;
  }
  else{
    //stepper.speed(polovina) - polovina normalni rychlosti
    do{
      motorStep(OPEN);
    }while(digitalRead(BTNOPEN));
    steps = MAXSTEPS;
  }
  //stepper.speed(normal) - zpet na normalni rychlost
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

