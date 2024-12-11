#include <UtilsWifi.h>
// Define htmlForm and server in the .cpp file
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

WebServer server(80); // Define the server instance here

// Save WiFi credentials to EEPROM
void saveWiFiCredentials(String ssid, String password) {
  EEPROM.begin(512);
  EEPROM.writeString(0, ssid);
  EEPROM.writeString(100, password);
  EEPROM.commit();
  Serial.println("WiFi credentials saved to EEPROM.");
}

// Load WiFi credentials from EEPROM
String loadWiFiSSID() {
  EEPROM.begin(512);
  String ssid = EEPROM.readString(0);
  return ssid;
}

String loadWiFiPassword() {
  EEPROM.begin(512);
  String password = EEPROM.readString(100);
  return password;
}

// Connect to WiFi with saved credentials
void connectToWiFi(String ssid, String password) {
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("Connecting to WiFi...");
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

// Handle root request (serve HTML form)
void handleRoot() {
  server.send(200, "text/html", htmlForm);
}

// Handle form submission (save credentials and connect to WiFi)
void handleFormSubmit() {
  String wifiName = server.arg("wifi_name");
  String wifiPassword = server.arg("password");

  Serial.println("Received WiFi credentials:");
  Serial.print("SSID: ");
  Serial.println(wifiName);
  Serial.print("Password: ");
  Serial.println(wifiPassword);



  // Try to connect to the WiFi network
  connectToWiFi(wifiName, wifiPassword);

  // Send response to user
  if (WiFi.status() == WL_CONNECTED) {
    server.send(200, "text/html", "<h2>Connected to WiFi! Please check the Serial Monitor for details.</h2>");
    startCameraServer();  // Start the camera server once connected to WiFi
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
    // Save the WiFi credentials to EEPROM
    saveWiFiCredentials(wifiName, wifiPassword);
  } else 
  {
    server.send(200, "text/html", "<h2>Failed to connect. Try again.</h2>");
  }
}
// Start Access Point
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
    // Load stored WiFi credentials
    String ssid = loadWiFiSSID();
    String password = loadWiFiPassword();

    if (ssid != "" && password != "") {
        // If credentials are found, attempt to connect to WiFi
        Serial.println("Found saved WiFi credentials, attempting to connect...");

        bool connected = false;
        int retryCount = 0;
        const int maxRetries = 5; // Number of retry attempts

        while (retryCount < maxRetries && !connected) {
            connectToWiFi(ssid, password); // Attempt to connect
            if (WiFi.status() == WL_CONNECTED) {
                connected = true;
                Serial.println("WiFi connected!");
                Serial.print("IP Address: ");
                Serial.println(WiFi.localIP());
                startCameraServer(); // Start the camera server
                Serial.print("Camera Ready! Use 'http://");
                Serial.print(WiFi.localIP());
                Serial.println("' to connect");
                return; // Exit the function once connected
            } else {
                retryCount++;
                Serial.printf("Failed to connect, retrying (%d/%d)...\n", retryCount, maxRetries);
                delay(5000); // Wait before retrying
            }
        }

        // If all retries fail, switch to Access Point mode
        if (!connected) {
            Serial.println("Unable to connect to WiFi after several attempts.");
            Serial.println("Switching to Access Point mode...");
            startAccessPoint();
        }
    } else {
        // No credentials found, start Access Point mode
        Serial.println("No saved WiFi credentials found. Switching to Access Point mode...");
        startAccessPoint();
    }
}
