#include "KeyPadUtils.h"

char getKey() {
    return keypad1.getKey();
}

String getString(char end_char='#') {
  char key = NO_KEY;
  String str;
  while(key != end_char) {
    key = getKey();
    if (key != NO_KEY) {
      str.concat(key);
      clearLCD();
      printToLCD(str, 0, 0);
    }
    delay(100);
  }
  return str;
}