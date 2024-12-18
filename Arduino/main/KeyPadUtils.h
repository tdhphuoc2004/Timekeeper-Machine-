#ifndef KEYPAD_UTILS_H
#define KEYPAD_UTILS_H

#include <Arduino.h>
#include <Keypad.h>
#include "LCD.h"
const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
//keymap defines the key pressed according to the row and columns just as appears on the keypad
const char mymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
const byte rowPin[numRows] = {12,11,4,9}; //Rows 0 to 3
const byte colPin[numCols]= {8,7,6,5}; //Columns 0 to 3

const Keypad keypad1= Keypad(makeKeymap(mymap), rowPin, colPin, numRows, numCols);
char getKey();
String getString(char end_char='#');

#endif // KEYPAD_UTILS_H