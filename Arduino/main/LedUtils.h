#ifndef LED_UTILS_H
#define LED_UTILS_H

#include <Arduino.h>

// LED pin definitions
#define GREEN_LED_PIN A0

// Functions to control the LEDs
void initLEDs();
void signalOK(); 
void signalFalse(); 

#endif // LED_UTILS_H
