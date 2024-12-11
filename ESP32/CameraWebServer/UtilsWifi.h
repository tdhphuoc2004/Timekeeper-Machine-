#include <WiFi.h>
#include <EEPROM.h> 
#include <WebServer.h>
// Declare htmlForm and server as extern variables
extern const char* htmlForm;
extern WebServer server;
void startCameraServer();
void setupLedFlash(int pin);
void saveWiFiCredentials(String ssid, String password); 
String loadWiFiSSID(); 
String loadWiFiPassword(); 
void connectToWiFi(String ssid, String password); 
void handleFormSubmit(); 
void handleRoot(); 
void startAccessPoint(); 
void initializeWiFi(); 