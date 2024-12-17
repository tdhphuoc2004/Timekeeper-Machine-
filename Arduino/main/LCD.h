#ifndef LCD_UTILS_H
#define LCD_UTILS_H

#include <Arduino.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// LCD pin definitions
#define RED_LCD_PIN 7
#define GREEN_LCD_PIN 6
void initializeLCD();
void turnOnBacklight();
void turnOffBacklight();
void printToLCD(String message, int col, int row);

#endif // LCD_UTILS_H
