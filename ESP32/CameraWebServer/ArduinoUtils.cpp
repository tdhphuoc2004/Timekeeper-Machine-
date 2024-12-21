#include "ArduinoUtils.h"
// Send a command to the Arduino
void sendToArduino(String message) {
  String modifiedMessage = "@"+message+"#";
  Serial.print(modifiedMessage);
}

String receiveFromArduino() {
  return Serial.readStringUntil('#');
}