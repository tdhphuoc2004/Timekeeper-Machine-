#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
void saveWiFiCredentials(String ssid, String password);
std::vector<std::pair<String, String>> loadWiFiCredentials();
void connectToWiFi(String ssid, String password);
bool connectToStoredWiFi();
void handleRoot();
void handleFormSubmit();
void startAccessPoint();
void initializeWiFi();
void handleServerRequests();
void startCameraServer();
void setupLedFlash(int pin);
void clearEEPROM(); 
void sendDebugMessage(String message); 
String checkIn(String id);
String receiveMessageFromServer(String serverURL); 

#endif // WIFI_UTILS_H
