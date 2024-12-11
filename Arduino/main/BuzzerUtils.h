#ifndef BUZZER_UTILS_H
#define BUZZER_UTILS_H

#include <Arduino.h>

// Buzzer pin definition
#define BUZZER_PIN 5

// Buzzer states
enum BuzzerState {
    BUZZER_OFF,
    BUZZER_OK,
    BUZZER_ERROR
};

// Function prototypes
void initializeBuzzer();
void setBuzzerState(BuzzerState state);

#endif // BUZZER_UTILS_H
