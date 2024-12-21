#include <Arduino.h>
#include "RFIDUtils.h"
#include "LedUtils.h"
#include "BuzzerUtils.h"
#include "KeyPadUtils.h"
#include "LCD.h"
#include "Esp32.h"
#include "Handle.h"
#include "config.h"
// #define NO_KEY '\0'  // Define NO_KEY as null character
SoftwareSerial espSerial(2, 3); // RX = Pin 2, TX = Pin 3 for ESP32-CAM


void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);  // For debugging
  initializeLCD();
  espSerial.flush();
   Serial.println("Arduino ready for communication.");
}

void loop() {
  // // handleInputID();
  // char key = getKey();  // Use function call to get key
  // if(key != NO_KEY) {
  //   Serial.println(key);
  //   // clearLCD();
  //   // printToLCD(String(key), 0, 0);
  // }
  // // delay(100);
  // if (key == '#') {
  //    Serial.println("Welcome");
  //   clearLCD();
  //   printToLCD("Choose options:", 0, 0);
  //   do {
  //     key = getKey();
  //     delay(100);
  //   } while(key < '1' or key > '3');
  //   if (key == '1') 
  //   {
  //     delay(100);
  //     clearLCD();
  //     printToLCD("Option 1", 0, 0);
  //     String res = RFIDtest('r', 1, "");
  //     Serial.println(res);
  //     setBuzzerState(BUZZER_OK); 
  //     setBuzzerState(BUZZER_ERROR); 
  //     setBuzzerState(BUZZER_ALERT); 
  //     sendToEsp32(res); 
  //     Serial.print(receiveFromEsp32());  
  
  //   } else if (key == '2') {
  //     if(handleInputID()) {
  //       clearLCD();
  //       printToLCD("Success", 0, 0);
  //     } else {
  //       clearLCD();
  //       printToLCD("Fail", 0, 0);
  //     }
  //   } else if (key == '3') {
  //     clearLCD();
  //     Serial.println("gg");
  //     printToLCD("Option 3", 0, 0);
  //     sendToEsp32("Face");
  //     delay(9000);
  //     Serial.print(receiveFromEsp32());  
      
  //   } 
    
  // }
  // delay(100);

    // Check for data from ESP32-CAM
  if (espSerial.available() > 0) {
      String receivedMessage = "";
      while (espSerial.available() > 0) {
        char incomingChar = espSerial.read();
        if (incomingChar != -1) {
          receivedMessage += incomingChar;
        }
      }
      Serial.println("Message from ESP32-CAM: " + receivedMessage);


    // Optionally, send data to ESP32-CAM
    if (Serial.available() > 0) {
      String messageToSend = Serial.readStringUntil('\n');
      espSerial.println(messageToSend);
      Serial.println("Sent to ESP32-CAM: " + messageToSend);
      }
  }
}
