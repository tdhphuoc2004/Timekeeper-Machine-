#include "BuzzerUtils.h"

// Initialize the buzzer pin
void initializeBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off initially
}

// Set the buzzer state
void setBuzzerState(BuzzerState state) {
    switch (state) {
        case BUZZER_OFF:
            noTone(BUZZER_PIN); // Turn off the buzzer
            break;

        case BUZZER_OK:
            tone(BUZZER_PIN, 1000, 200); // Play a short tone (1 kHz for 200 ms)
            delay(200);
            noTone(BUZZER_PIN); // Stop the buzzer
            break;

        case BUZZER_ERROR:
            for (int i = 0; i < 3; i++) { // Play 3 short error beeps
                tone(BUZZER_PIN, 500, 200); // Play a tone (500 Hz for 200 ms)
                delay(300);                 // Wait between beeps
            }
            noTone(BUZZER_PIN); // Stop the buzzer
            break;
    }
}
