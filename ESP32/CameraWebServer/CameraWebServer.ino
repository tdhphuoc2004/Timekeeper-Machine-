#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#define CAMERA_MODEL_AI_THINKER 
#include "camera_pins.h"
#include "UtilsWifi.h"
#include "ArduinoUtils.h"
#include <HardwareSerial.h>
const char* serverUrl = "http://192.168.1.14:8080/upload";
// Function to send debug messages to the HTTP server
void sendDebugMessage(String message) {
  HTTPClient http;

  String serverURL = "http://10.1.1.249:5001/api/debug"; // Replace with your debug server URL

  http.begin(serverURL);       // Initialize HTTP connection
  http.addHeader("Content-Type", "application/json"); // Set content type to JSON

  String jsonPayload = "{\"message\":\"" + message + "\"}"; // Create JSON payload
  int httpResponseCode = http.POST(jsonPayload);           // Send POST request

  if (httpResponseCode > 0) {
    Serial.printf("HTTP POST successful, response code: %d\n", httpResponseCode);
  } else {
    Serial.printf("HTTP POST failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end(); // Close HTTP connection
}

// Function to receive messages from the server
String receiveMessageFromServer(String serverURL) {
  HTTPClient http;
  String response = "";

  // Initialize HTTP connection
  http.begin(serverURL);
  response = http.getString();
  Serial.println("Server response: " + response);


  http.end(); // Close HTTP connection
  return response;
}

void setup() {
  // Serial1.begin(9600);    // Communication with Arduino
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
  config.jpeg_quality = 8;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 8;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_VGA;
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
    s->set_framesize(s, FRAMESIZE_HVGA);
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
//================================================= CODE HERE ========================================================
   initializeWiFi();
  // Serial.begin(115200);                // Debugging via USB
  // Serial1.begin(9600, SERIAL_8N1, 3, 1); // Serial1 for communication with Arduino
  Serial.println("ESP32-CAM ready for bidirectional communication.");
   //clearEEPROM(); 
}

void sendImage() {
  String boundary = "----ESP32Boundary";
  String payload = "--" + boundary + "\r\n";
  payload += "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n";
  payload += "Content-Type: image/jpeg\r\n\r\n";

  WiFiClient client;
  HTTPClient http;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
      Serial.println("Camera capture failed");
      return;
  }

  // Create the HTTP payload
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

  int httpResponseCode = http.POST(payload + String((char *)fb->buf, fb->len) + "\r\n--" + boundary + "--\r\n");

  if (httpResponseCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      String response = http.getString();
      Serial.println("Server response: " + response);
      
  } else {
      Serial.printf("HTTP POST failed: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  esp_camera_fb_return(fb);
  http.end();
}


void loop() 
{
  handleServerRequests();
  // // Send a message to Arduino every 2 seconds
  // static unsigned long lastTime = 0;
  // if (millis() - lastTime > 2000) {
  //   lastTime = millis();
  //   String messageToArduino = "Hello from ESP32-CAM!";
  //   Serial1.println(messageToArduino); // Send message to Arduino
  //   Serial.println("Sent to Arduino: " + messageToArduino); // Debug
  //   sendDebugMessage("Sent to Arduino: " + messageToArduino);
  // }

  // // Check for messages from Arduino
  // if (Serial1.available() > 0) {
  //   String messageFromArduino = Serial1.readStringUntil('\n'); // Read message from Arduino
  //   Serial.println("Received from Arduino: " + messageFromArduino); // Debug
  //   sendDebugMessage("Sent to Arduino: " + messageFromArduino);
  // }
  
  // String serverURL = "http://10.1.1.249:5001/api/debug";
  // sendDebugMessage("dmmm"); 
  // String serverResponse = receiveMessageFromServer(serverURL);
  // Serial.println(serverResponse); 
  // sendImage();
  delay(500);  
}