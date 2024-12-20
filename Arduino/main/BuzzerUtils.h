#ifndef BUZZER_UTILS_H
#define BUZZER_UTILS_H

#include <Arduino.h>

// Buzzer pin definition
#define BUZZER_PIN A3

// Buzzer states
enum BuzzerState {
    BUZZER_OFF,
    BUZZER_OK,
    BUZZER_ERROR,
    BUZZER_ALERT
};

// Function prototypes
void initializeBuzzer();
void setBuzzerState(BuzzerState state);

#endif // BUZZER_UTILS_H
