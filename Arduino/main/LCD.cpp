#include "LCD.h"
#include <LiquidCrystal_I2C.h>
// Initialize LCD pins
void initializeLCD() {
    lcd.init();
    lcd.noBacklight();
}

void turnOnBacklight() {
    lcd.backlight();
}

void turnOffBacklight() {
    lcd.noBacklight();
}

void printToLCD(String message, int col, int row) {
    lcd.clear();
    lcd.setCursor(col, row);
    lcd.print(message);
}