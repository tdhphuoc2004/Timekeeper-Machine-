#ifndef PHOTORESISTOR_UTILS_H
#define PHOTORESISTOR_UTILS_H

#include <Arduino.h>

bool isLightAboveThreshold(int sensorPin, int threshold);
int getPhotoValue(int sensorPin);

#endif
