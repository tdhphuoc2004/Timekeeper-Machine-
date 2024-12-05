#include "esp_camera.h"
#include <WiFi.h>
#include <EEPROM.h> 
#include <WebServer.h>
//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//define CAMERA_MODEL_ESP_EYE  // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT  // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"
void startCameraServer();
void setupLedFlash(int pin);
// Create web server on port 80
WebServer server(80);
// HTML form for WiFi credentials
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

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif
 // Load stored WiFi credentials
  String ssid = loadWiFiSSID();
  String password = loadWiFiPassword();

  // Check if stored WiFi credentials exist
  if (ssid != "" && password != "") 
  {
    // If credentials are found, attempt to connect to WiFi
    Serial.println("Found saved WiFi credentials, attempting to connect...");

    bool connected = false;
    int retryCount = 0;
    int maxRetries = 5;  // Number of retry attempts

    while (retryCount < maxRetries && !connected) {
      connectToWiFi(ssid, password);  // Try to connect
      if (WiFi.status() == WL_CONNECTED) {
        connected = true;
        startCameraServer();  // Start the camera server
        Serial.print("Camera Ready! Use 'http://");
        Serial.print(WiFi.localIP());
        Serial.println("' to connect");
      } else {
        retryCount++;
        Serial.print("Failed to connect, retrying... (");
        Serial.print(retryCount);
        Serial.print(" of ");
        Serial.print(maxRetries);
        Serial.println(")");
        delay(5000);  // Wait before retrying (5 seconds)
      }
    }

    if (!connected) 
    {
      // If connection fails after retries, notify the user
      Serial.println("Unable to connect to WiFi after several attempts. Please check your network settings.");
      Serial.println("Switching to Access Point mode...");
      WiFi.softAP("ESP32-Access-Point", "123456789");
      Serial.println("Access Point Started");
      Serial.print("IP Address: ");
      Serial.println(WiFi.softAPIP());  // Show AP IP address
      server.on("/", HTTP_GET, handleRoot);  // Serve the WiFi configuration form
      server.on("/submit", HTTP_POST, handleFormSubmit);  // Handle form submission
      server.begin();  // Start the web server
    }

  }
  else 
  {
    // If no credentials are found, start as Access Point and serve the form
    Serial.println("No saved WiFi credentials found. Switching to Access Point mode...");
    WiFi.softAP("ESP32-Access-Point", "123456789");
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());  // Show AP IP address
    server.on("/", HTTP_GET, handleRoot);  // Serve the WiFi configuration form
    server.on("/submit", HTTP_POST, handleFormSubmit);  // Handle form submission
    server.begin();  // Start the web server
  }
}

void loop() {
  delay(10000);
  server.handleClient();
}
