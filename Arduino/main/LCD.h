#ifndef LCD_H
#define LCD_H
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
const LiquidCrystal_I2C lcd(0x27, 16, 2);
// LCD pin definitions
void initializeLCD();
void turnOnBacklight();
void turnOffBacklight();
void printToLCD(String message, int col=0, int row=0);
void clearLCD();
#endif // LCD_H
