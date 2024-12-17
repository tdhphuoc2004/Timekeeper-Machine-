#include <Arduino.h>
#include "RFIDUtils.h"
#include "LedUtils.h"
#include "BuzzerUtils.h"
#include "KeyPadUtils.h"
#include "LCD.h"

#define NO_KEY '\0'  // Define NO_KEY as null character



void setup() {
  Serial.begin(9600);  // For debugging
  initializeLCD();
}

void loop() {
  String key = getKey();  // Use function call to get key

  if (key != NO_KEY) {  // If a key is pressed
    Serial.println(key);
    lcd.print(key);
  }
}
