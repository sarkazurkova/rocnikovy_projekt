#ifndef motor_h
#define motor_h
#include <Arduino.h>


void motorStep(bool dir);
void ledState(int ledTime);
void ledEnds();
//void doorMove(unsigned long value);
void doorPosition();

#endif