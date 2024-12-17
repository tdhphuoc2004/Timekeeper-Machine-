#ifndef LCD_UTILS_H
#define LCD_UTILS_H

#include <Arduino.h>
#include <Keypad.h>

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte rowPins[numRows] = {12,11,4,9}; //Rows 0 to 3
byte colPins[numCols]= {8,7,6,5}; //Columns 0 to 3

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
char getKey();

#endif // LCD_UTILS_H