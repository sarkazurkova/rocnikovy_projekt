#include <Arduino.h>
#include <IRremote.hpp>


#define IRDioda 3
#define IRSensor 2
#define BTN 8

bool state = 0;
#define ROUNDS 20

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(IRSensor, INPUT);
  IrReceiver.begin(IRDioda, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
}

void motorStep(bool dir){
    Serial.print("+ STEP TO ");
    if(dir){
      Serial.println("RIGHT!");
    }else{
      Serial.println("LEFT!");
    }
}

void motorStart(bool dir){
  for(int i = 0; i < ROUNDS; ++i){
    Serial.print(i);
    Serial.print(". STEP TO ");
    if(dir){
      Serial.println("RIGHT!");
    }else{
      Serial.println("LEFT!");
    }
  }
}

void loop() {
 if (IrReceiver.decode()){
    unsigned long receivedValue = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(receivedValue);
    Serial.println(receivedValue, HEX);
    IrReceiver.printIRResultShort(&Serial);
    if(receivedValue==2991456000){
      motorStart(state);
      do{
        motorStep(state);
      }while (digitalRead(IRSensor) && !digitalRead(BTN));
      if(digitalRead(BTN)){
        state = !state;
      }
    }
    
    Serial.print("STATE: ");
    Serial.println(state);
    IrReceiver.resume();
  }
  /*if(digitalRead(BTN)){
      Serial.println("BTN!");
    }*/
}

