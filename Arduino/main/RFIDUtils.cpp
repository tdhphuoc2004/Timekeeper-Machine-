#include "RFIDUtils.h"

// Create MFRC522 instance
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

// Initialize RFID reader
void initializeRFID() {
    SPI.begin();            // Initialize SPI bus
    rfid.PCD_Init();        // Initialize the RFID reader
    Serial.println("RFID Reader Initialized. Place a card near the reader...");

    // Initialize default key for authentication
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

// Write a string to a specific block on the RFID card
void writeDataToCard(byte block, String data) {
    byte buffer[16];
    data.getBytes(buffer, 16); // Convert the string to bytes (up to 16 bytes)

    // Authenticate the block
    if (rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid)) != MFRC522::STATUS_OK) {
        Serial.println("Authentication failed!");
        return;
    }

    // Write to the block
    if (rfid.MIFARE_Write(block, buffer, 16) != MFRC522::STATUS_OK) {
        Serial.println("Writing to block failed!");
    } else {
        Serial.println("Data written to card successfully.");
    }
}

// Read a string from a specific block on the RFID card
String readDataFromCard(byte block) {
    byte buffer[18];
    byte bufferSize = sizeof(buffer);

    // Authenticate the block
    if (rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid)) != MFRC522::STATUS_OK) {
        Serial.println("Authentication failed!");
        return "";
    }

    // Read the block
    if (rfid.MIFARE_Read(block, buffer, &bufferSize) != MFRC522::STATUS_OK) {
        Serial.println("Reading from block failed!");
        return "";
    }

    // Convert the data to a string
    String result = "";
    for (byte i = 0; i < 16; i++) {
        if (buffer[i] != 0) { // Ignore padding bytes
            result += (char)buffer[i];
        }
    }
    return result;
}
void deleteDataFromCard(byte block) {
    byte emptyBuffer[16] = {0}; // Array of 16 bytes filled with 0

    // Authenticate the block
    if (rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid)) != MFRC522::STATUS_OK) {
        Serial.println("Authentication failed!");
        return;
    }

    // Write empty data to the block
    if (rfid.MIFARE_Write(block, emptyBuffer, 16) != MFRC522::STATUS_OK) {
        Serial.println("Failed to write empty data to the block!");
    } else {
        Serial.println("Data deleted successfully.");
    }
}
void printCardData() {
    // Iterate over all sectors
    for (byte sector = 0; sector < 16; sector++) { // Typical MIFARE 1K has 16 sectors
        Serial.print("Sector ");
        Serial.println(sector);

        // Iterate over all blocks in the sector
        for (byte block = 0; block < 4; block++) { // Each sector has 4 blocks
            byte blockAddress = sector * 4 + block;
            byte buffer[18];  // Data buffer for reading
            byte bufferSize = sizeof(buffer);

            // Authenticate block
            if (rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddress, &key, &(rfid.uid)) != MFRC522::STATUS_OK) {
                Serial.print("Failed to authenticate block ");
                Serial.println(blockAddress);
                continue;
            }

            // Read block data
            if (rfid.MIFARE_Read(blockAddress, buffer, &bufferSize) != MFRC522::STATUS_OK) {
                Serial.print("Failed to read block ");
                Serial.println(blockAddress);
                continue;
            }

            // Print block data in hexadecimal
            Serial.print(" Block ");
            Serial.print(block);
            Serial.print(": ");
            for (byte i = 0; i < 16; i++) {
                Serial.print(buffer[i] < 0x10 ? " 0" : " "); // Add leading zero for single-digit hex numbers
                Serial.print(buffer[i], HEX);
            }
            Serial.println();
        }
        Serial.println("--------------------------");
    }
    rfid.PCD_StopCrypto1(); // Stop encryption after reading all blocks
}

