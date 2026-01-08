#include <Arduino.h>
#include <Stepper.h> 
#include "motor.h"
#include "eeprom.h"
#include "definitions.h"

Stepper stepper(STEPS, 4, 6, 5, 7);

extern bool state;
extern int steps;
const int oneStep = 15;

void motorStep(bool dir){
    if(dir){
      stepper.step(oneStep);
      steps++;
    }else{
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
