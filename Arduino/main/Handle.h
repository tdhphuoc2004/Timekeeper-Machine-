#ifndef HANDLE_H
#define HANDLE_H

#include <Arduino.h>
#include "KeyPadUtils.h"
#include "Esp32.h"

bool handleInputID();
void sendToEsp32(String message);

#endif // HANDLE_H