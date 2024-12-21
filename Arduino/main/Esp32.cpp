#include "Esp32.h"

void initializeEsp32() {
  Serial.begin(9600);
  espSerial.begin(115200);
}
void sendToEsp32(String message) {
  String modifiedMessage = "@" + message + "#"; 
  Serial.println(modifiedMessage);
  espSerial.flush();
  espSerial.println(modifiedMessage);
}

String receiveFromEsp32() {
  char receive = '#'; 
  while (receive != '@')
  {
    receive = espSerial.read();
  }

  return espSerial.readStringUntil('#');
}