#ifndef LED_UTILS_H
#define LED_UTILS_H

#include <Arduino.h>

// LED pin definitions
#define RED_LED_PIN 7
#define GREEN_LED_PIN 6

// Functions to control the LEDs
void initializeLEDs();
void turnOnRedLED();
void turnOnGreenLED();
void turnOffLEDs();

#endif // LED_UTILS_H
