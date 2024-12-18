#ifndef RFID_UTILS_H
#define RFID_UTILS_H

#include <SPI.h>
#include <MFRC522.h>

// Define RFID Pins
#define SS_PIN 10 // SDA
#define RST_PIN 9 // RST

// Declare RFID instance and key
extern MFRC522 rfid;
extern MFRC522::MIFARE_Key key;

// Function declarations
//void initializeRFID();
void writeDataToCard(byte block, String data);
String readDataFromCard(byte block);
void deleteDataFromCard(byte block);
void printCardData();
//void deinitializeRFID();
void testRFID();
String RFIDtest(char operation, byte block, String data = ""); 

#endif // RFID_UTILS_H
