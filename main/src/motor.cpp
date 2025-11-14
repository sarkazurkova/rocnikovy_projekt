#include <Arduino.h>
#include <Stepper.h> 
#include "motor.h"
#include "eeprom.h"
#include "definitions.h"

Stepper stepper(STEPS, 4, 6, 5, 7);

extern bool state;
extern int steps;
const int oneStep = 10;

void motorStep(bool dir){
    Serial.print("+ STEP TO ");
    
    if(dir){
      Serial.println("RIGHT!");
      stepper.step(oneStep);
      steps++;
    }else{
      Serial.println("LEFT!");
      stepper.step(-oneStep);
      steps--;
    }
}


void ledState(int ledTime){
  if(ledTime%10==0){
      state = !state;
      digitalWrite(LED, state);
  }
}

void ledEnds(){
  state = LOW;
  digitalWrite(LED, state);
}


void doorPosition(){
  if(digitalRead(BTNOPEN)){
    steps = 0;
  }
  if(digitalRead(BTNOPEN)){
    steps = MAXSTEPS;
  }
  else{
    do{
      motorStep(1);
    }while(digitalRead(BTNOPEN));
    steps = MAXSTEPS;
  }
  
}