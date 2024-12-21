#include "Handle.h"

bool handleInputID() {
  String id = getString('#');
  // String id = "123456#";
  
  sendToEsp32(id);
  delay(100);
  String messageFromESP32 = receiveFromEsp32();
  if (messageFromESP32 == "OK") return true;
  else {
    return false;
  }
}