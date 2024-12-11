#include <Arduino.h>
#include "RFIDUtils.h"
#include "LedUtils.h"
#include "BuzzerUtils.h"

void setup() {
    Serial.begin(115200); // Initialize serial communication
    initializeRFID();     // Initialize RFID reader
    initializeLEDs();     // Initialize LEDs
    initializeBuzzer();   // Initialize Buzzer

    Serial.println("Deleting data from RFID card. Please present a card...");

    unsigned long startTime = millis();
    bool cardProcessed = false;

    while (millis() - startTime < 10000) { // 10-second timeout
        if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
            Serial.println("Card detected! Deleting data...");
            
            // Delete data from the card
            deleteDataFromCard(1); // Clear data from block 1
            Serial.println("Data deleted successfully!");

            turnOnGreenLED();          // Indicate success with the green LED
            setBuzzerState(BUZZER_OK); // Indicate success with a sound

            rfid.PICC_HaltA();          // Halt the card
            rfid.PCD_StopCrypto1();     // Stop encryption
            cardProcessed = true;
            break;                      // Exit the loop after processing the card
        }

        delay(500); // Delay to prevent flooding the card reader
    }

    if (!cardProcessed) {
        Serial.println("Timeout! No card was processed.");
        setBuzzerState(BUZZER_ERROR); // Indicate timeout error
        turnOnRedLED();
    }
}

void loop() {
    // The loop can remain empty or perform other operations
    Serial.println("Waiting for other tasks...");
    delay(1000); // Example delay
}
