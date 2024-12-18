#include "Esp32.h"

void initializeEsp32() {
  Serial.begin(9600);
  espSerial.begin(115200);
}
void sendToEsp32(String message) {
  espSerial.println(message);
}

String receiveFromEsp32() {
  // Serial.println(espSerial.read());
  // char message = espSerial.read();
  // return String(message);
  delay(100);
  return espSerial.readStringUntil('\n');
}