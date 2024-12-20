#include "ArduinoUtils.h"
// Send a command to the Arduino
void sendToArduino(String message) {
  message += "#";
  Serial.print(message);
}

String receiveFromArduino() {
  delay(100);
  
  return Serial.readStringUntil('#');
}