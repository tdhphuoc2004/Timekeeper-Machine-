#include "photoresistor.h"

// Check if photoresistor reading is above threshold
bool isLightAboveThreshold(int sensorPin, int threshold) {
    int sensorValue = analogRead(sensorPin);
    return (sensorValue > threshold);
}

int getPhotoValue(int sensorPin) {
  return analogRead(sensorPin);
}