#ifndef ESP32_H
#define ESP32_H

#include <Arduino.h>
#include <SoftwareSerial.h>
const SoftwareSerial espSerial(2, 3); // RX = Pin 2, TX = Pin 3 for ESP32-CAM
void initializeEsp32();
void sendToEsp32(String message);
String receiveFromEsp32();
#endif // ESP32_H