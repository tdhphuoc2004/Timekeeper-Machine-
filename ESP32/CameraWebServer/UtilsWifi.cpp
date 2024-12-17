#include "UtilsWifi.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <WebServer.h>

const char* htmlForm = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WiFi Configuration</title>
</head>
<body>
    <h2>Enter WiFi Name and Password</h2>
    <form action="/submit" method="POST">
        <label for="wifi_name">WiFi Name:</label>
        <input type="text" id="wifi_name" name="wifi_name" required><br><br>
        
        <label for="password">Password:</label>
        <input type="password" id="password" name="password" required><br><br>
        
        <input type="submit" value="Connect">
    </form>
</body>
</html>
)";

WebServer server(80); // HTTP server instance

void clearEEPROM() {
    EEPROM.begin(512); // Ensure the EEPROM size matches the size used in your application

    for (int i = 0; i < 512; i++) { // Loop through all EEPROM addresses
        EEPROM.write(i, 0);        // Write 0 to each address
    }

    EEPROM.commit(); // Commit changes to EEPROM
    Serial.println("EEPROM has been cleared.");
}

void saveWiFiCredentials(String ssid, String password) {
    EEPROM.begin(512);
    int offset = 0;
    for (int i = 0; i < 5; i++) { // Store up to 5 WiFi credentials
        String existingSSID = EEPROM.readString(offset);
        if (existingSSID == "") { // Empty slot found
            EEPROM.writeString(offset, ssid);
            EEPROM.writeString(offset + 100, password);
            EEPROM.commit();
            Serial.println("WiFi credentials saved to EEPROM.");
            return;
        }
        offset += 200; // Move to the next slot
    }
    Serial.println("No space to save more WiFi credentials.");
}

std::vector<std::pair<String, String>> loadWiFiCredentials() {
    EEPROM.begin(512);
    std::vector<std::pair<String, String>> credentials;
    int offset = 0;
    for (int i = 0; i < 5; i++) {
        String ssid = EEPROM.readString(offset);
        String password = EEPROM.readString(offset + 100);
        if (ssid != "") {
            credentials.push_back(std::make_pair(ssid, password));
        }
        offset += 200; // Move to the next slot
    }
    return credentials;
}

void connectToWiFi(String ssid, String password) {
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        startCameraServer(); 
    } else {
        Serial.println("\nFailed to connect to WiFi.");
    }
}

bool connectToStoredWiFi() {
    auto credentials = loadWiFiCredentials();
    for (const auto& cred : credentials) {
        String ssid = cred.first;
        String password = cred.second;

        connectToWiFi(ssid, password);
        if (WiFi.status() == WL_CONNECTED) {
            return true; // Successfully connected
        }
    }
    return false; // All attempts failed
}

void handleRoot() {
    server.send(200, "text/html", htmlForm);
}

void handleFormSubmit() {
    String wifiName = server.arg("wifi_name");
    String wifiPassword = server.arg("password");

    Serial.println("Received WiFi credentials:");
    Serial.print("SSID: ");
    Serial.println(wifiName);
    Serial.print("Password: ");
    Serial.println(wifiPassword);

    connectToWiFi(wifiName, wifiPassword);
    if (WiFi.status() == WL_CONNECTED) {
        startCameraServer(); 
        saveWiFiCredentials(wifiName, wifiPassword);
        server.send(200, "text/html", "<h2>Connected to WiFi! Check Serial Monitor for details.</h2>");
        Serial.print("Camera Ready! Use 'http://");
        Serial.print(WiFi.localIP());
        Serial.println("' to connect");
    } else {
        server.send(200, "text/html", "<h2>Failed to connect. Try again.</h2>");
    }
}

void startAccessPoint() {
    WiFi.softAP("ESP32-Access-Point", "123456789");
    Serial.println("Access Point started.");
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, handleRoot);
    server.on("/submit", HTTP_POST, handleFormSubmit);
    server.begin();
}

void initializeWiFi() {
    if (connectToStoredWiFi()) {
        Serial.print("Connected to WiFi! Use 'http://");
        Serial.print(WiFi.localIP());
        Serial.println("' to access.");
    } else {
        Serial.println("Failed to connect to all stored WiFi networks.");
        startAccessPoint(); // Switch to AP mode
    }
}

void handleServerRequests() {
    server.handleClient();
}