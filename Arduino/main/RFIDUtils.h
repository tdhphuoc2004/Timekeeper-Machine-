#ifndef RFID_UTILS_H
#define RFID_UTILS_H

#include <SPI.h>
#include <MFRC522.h>

// Define RFID Pins
#define SS_PIN 10 // SDA
#define RST_PIN 9 // RST

// Extern declaration of rfid object
extern MFRC522 rfid;

// Function declarations
void initializeRFID();
void writeDataToCard(byte block, String data);
String readDataFromCard(byte block);
void deleteDataFromCard(byte block); 
void printCardData();
#endif
