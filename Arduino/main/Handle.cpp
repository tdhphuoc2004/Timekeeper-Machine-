#include "Handle.h"

bool handleInputID() {
  String id = getString('#');
  // String id = "123456#";
  sendToEsp32(id);
  delay(1000);
  String messageFromESP32 = receiveFromEsp32();
  Serial.println(messageFromESP32);
  Serial.println(id);
  if (messageFromESP32 == id) return true;
  else {
    return false;
  }
}