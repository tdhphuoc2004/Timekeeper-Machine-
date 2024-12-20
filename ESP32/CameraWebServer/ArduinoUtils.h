#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

#include <Arduino.h> // Include the Arduino core library
#include "config.h"
// Function declarations
void sendToArduino(String message);

String receiveFromArduino();

#endif
