#ifndef ESP32_H
#define ESP32_H
#include <SoftwareSerial.h> 
#include <Arduino.h>
#include "config.h"
void initializeEsp32();
void sendToEsp32(String message);
String receiveFromEsp32();
#endif // ESP32_H