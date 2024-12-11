#include "LedUtils.h"

// Initialize LED pins
void initializeLEDs() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    turnOffLEDs(); // Ensure LEDs are off at initialization
    turnOnRedLED(); // Default to red LED on
}

// Turn on the red LED and turn off the green LED
void turnOnRedLED() {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
}

// Turn on the green LED and turn off the red LED
void turnOnGreenLED() {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
}

// Turn off both LEDs
void turnOffLEDs() {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
}
