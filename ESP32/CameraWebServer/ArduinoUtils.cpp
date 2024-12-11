#include "ArduinoUtils.h"
// Send a command to the Arduino
void sendCommandToArduino(String command) {
    Serial1.println(command); // Send the command over Serial1
    Serial.printf("Command sent to Arduino: %s\n", command.c_str());
}

// Wait for acknowledgment from Arduino
String waitForAcknowledgment() {
    unsigned long timeout = 2000; 
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        if (Serial1.available() > 0) {
            String response = Serial1.readStringUntil('\n');
            response.trim(); // Remove whitespace or newline characters
            return response; // Return acknowledgment
        }
    }
    return ""; // Return empty string if timeout
}