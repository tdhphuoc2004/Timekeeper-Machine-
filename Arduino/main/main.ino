#include <Arduino.h>
#include "RFIDUtils.h"
#include "LedUtils.h"
#include "BuzzerUtils.h"
#include "KeyPadUtils.h"
#include "LCD.h"
#include "Esp32.h"
#include "Handle.h"
// #define NO_KEY '\0'  // Define NO_KEY as null character



void setup() {
  initializeEsp32();  // For debugging
  initializeLCD();
}

void loop() {
  // handleInputID();
  char key = getKey();  // Use function call to get key
  // if(key != NO_KEY) {
  //   Serial.println(key);
  //   clearLCD();
  //   printToLCD(String(key), 0, 0);
  // }
  // delay(100);
  if (key == '#') {
    clearLCD();
    printToLCD("Choose options:", 0, 0);
    do {
      key = getKey();
      delay(100);
    } while(key < '1' or key > '3');
    if (key == '1') 
    {
      delay(100);
      clearLCD();
      printToLCD("Option 1", 0, 0);
      String res = RFIDtest('r', 1, ""); 
      res += '#'; 
      Serial.println(res);
      sendToEsp32(res);  
  
    } else if (key == '2') {
      if(handleInputID()) {
        clearLCD();
        printToLCD("OK", 0, 0);
      } else {
        clearLCD();
        printToLCD("Fail", 0, 0);
      }
    } else if (key == '3') {
      clearLCD();
      printToLCD("Option 3", 0, 0);
    } 
    
  }
  delay(100);

}
