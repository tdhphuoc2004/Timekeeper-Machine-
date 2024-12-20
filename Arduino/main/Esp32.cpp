#include "Esp32.h"

void initializeEsp32() {
  Serial.begin(9600);
  espSerial.begin(115200);
}
void sendToEsp32(String message) {
  message += "#";
  espSerial.println(message);
}

String receiveFromEsp32() {
  delay(100);
  
  return espSerial.readStringUntil('#');
}