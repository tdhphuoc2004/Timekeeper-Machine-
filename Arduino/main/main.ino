#include <Arduino.h>
#include "RFIDUtils.h"
#include "LedUtils.h"
#include "BuzzerUtils.h"
#include "KeyPadUtils.h"
#include "LCD.h"
#include "Esp32.h"
#include "Handle.h"
#include "config.h"
#include "photoresistor.h"
// #define NO_KEY '\0'  // Define NO_KEY as null character
SoftwareSerial espSerial(2, 3); // RX = Pin 2, TX = Pin 3 for ESP32-CAM
#define PHOTO_PIN       A1
#define LIGHT_THRESHOLD 500 

void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);  // For debugging
  initializeLCD();
  initLEDs(); 
  initializeBuzzer(); 
  espSerial.flush();
  Serial.println("Arduino ready for communication.");  
  if(isLightAboveThreshold(PHOTO_PIN, LIGHT_THRESHOLD))
  {
      lcd.backlight();
  }
  else lcd.noBacklight();
 
}

void loop() {
  // // handleInputID();
  char key = getKey();  // Use function call to get key
  if(key != NO_KEY) {
    Serial.println(key);
  }
  // delay(100);
  if (key == '#') {
    Serial.println("Welcome");
    clearLCD();
    printToLCD("Choose options:", 0, 0);
    do {
      key = getKey();
      delay(100);
    } while(key < '1' or key > '3');
    if (key == '1') 
    {
      delay(100);
      clearLCD();
      printToLCD("Option 1", 0, 0);
      String res = RFIDtest('r', 1, "");
      Serial.println(res);
      sendToEsp32(res); 
      String response = receiveFromEsp32(); 
      Serial.println("Response:");
      Serial.println(response); 
      printToLCD(res, 0, 0); 
      if (response == "OK")
      {
         printToLCD("Successful", 0, 1); 
         setBuzzerState(BUZZER_OK); 
          signalOK(); 
      }
      else
      {
         printToLCD("Fail", 0, 1); 
         setBuzzerState(BUZZER_ERROR); 
         signalFalse();
      }
  
    } else if (key == '2') 
    {
      if(handleInputID()) 
      {
        clearLCD();
        printToLCD("Successful", 0, 0);
        setBuzzerState(BUZZER_OK); 
        signalOK(); 
      }
      else 
      {
        clearLCD();
        printToLCD("Fail", 0, 0);
        setBuzzerState(BUZZER_ERROR); 
        signalFalse(); 
      }
    } else if (key == '3') {
      clearLCD();
      Serial.println("gg");
      printToLCD("Option 3", 0, 0);
      sendToEsp32("Face");
      delay(9000);
      Serial.print(receiveFromEsp32());  
      
    } 
    
  }
  delay(100);
  //  turnOnRedLED(); 
  //   delay(2000);
  //  turnOnGreenLED(); 
  //   delay(2000);
  //   turnOffLEDs(); 
  //   delay(2000);

}
