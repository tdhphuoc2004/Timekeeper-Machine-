#include "LCD.h"
// Initialize LCD pins
void initializeLCD() {
    lcd.init();
    lcd.noBacklight();
    lcd.clear();
}

void turnOnBacklight() {
    lcd.backlight();
}

void turnOffBacklight() {
    lcd.noBacklight();
}

void printToLCD(String message, int col, int row) {
    lcd.setCursor(col, row);
    lcd.print(message);
}

void clearLCD() {
  lcd.clear();
}