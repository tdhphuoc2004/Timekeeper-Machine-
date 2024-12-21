#include "LedUtils.h"

void initLEDs() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void signalOK() {
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(5000); 
  digitalWrite(GREEN_LED_PIN, LOW);
}

void signalFalse() {
  unsigned long start = millis();
  while (millis() - start < 5000) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(250);
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(250);
  }
}