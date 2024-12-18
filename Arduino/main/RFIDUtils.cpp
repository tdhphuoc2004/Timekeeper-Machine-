#include "RFIDUtils.h"
#include "LCD.h" 

// Define RFID instance and key
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

String RFIDtest(char operation, byte block, String data = "") {
    String result = ""; // Initialize the result string

    // Initialize RFID
    SPI.begin();
    rfid.PCD_Init();
    Serial.println("RFID Reader Initialized. Place a card near the reader...");

    // Initialize default key for authentication
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    // Wait for a card to be detected
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        Serial.println("No card detected. Please place a card near the reader.");
        SPI.end();
        return result;
    }

    // Print card UID for debugging
    Serial.print("Card UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Authenticate the block
    if (rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid)) != MFRC522::STATUS_OK) {
        Serial.println("Authentication failed!");
        SPI.end();
        return result;
    }

    // Perform the selected operation
    switch (operation) {
        case 'w': // Write operation
            if (data.length() > 0) {
                byte buffer[16];
                data.getBytes(buffer, 16); // Convert the string to bytes (up to 16 bytes)
                if (rfid.MIFARE_Write(block, buffer, 16) == MFRC522::STATUS_OK) {
                    Serial.println("Data written to card successfully.");
                } else {
                    Serial.println("Failed to write data to the block.");
                }
            } else {
                Serial.println("No data provided for writing.");
            }
            break;

        case 'r': // Read operation
            {
                byte buffer[18];
                byte bufferSize = sizeof(buffer);
                if (rfid.MIFARE_Read(block, buffer, &bufferSize) == MFRC522::STATUS_OK) {
                    Serial.print("Data in block (first 8 bytes): ");
                    for (byte i = 0; i < 8; i++) { // Read only the first 8 bytes
                        Serial.print((char)buffer[i]); // Print as characters for readability
                        result += (char)buffer[i];    // Append to the result string
                    }
                    Serial.println();
                } else {
                    Serial.println("Failed to read data from the block.");
                }
            }
            break;

        case 'd': // Delete operation
            {
                byte emptyBuffer[16] = {0}; // Array of 16 bytes filled with 0
                if (rfid.MIFARE_Write(block, emptyBuffer, 16) == MFRC522::STATUS_OK) {
                    Serial.println("Data in block deleted successfully.");
                } else {
                    Serial.println("Failed to delete data from the block.");
                }
            }
            break;

        default:
            Serial.println("Invalid operation. Use 'w' for write, 'r' for read, or 'd' for delete.");
    }

    // Halt card and stop encryption
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    // Reinitialize RFID for the next use
    SPI.end();
    Serial.println("RFID Reader Deinitialized.");

    return result; // Return the read string (empty if not a read operation)
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

