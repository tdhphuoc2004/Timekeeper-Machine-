#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

#include <Arduino.h> // Include the Arduino core library

// Function declarations
String waitForAcknowledgment();
void sendCommandToArduino(String command);

#endif
