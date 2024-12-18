#ifndef FACE_UTILS_H
#define FACE_UTILS_H
#include "MqttUtils.h"
#include <base64.h>

// void send_image();

// void sendImage() 
// {
//   String boundary = "----ESP32Boundary";
//   String payload = "--" + boundary + "\r\n";
//   payload += "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n";
//   payload += "Content-Type: image/jpeg\r\n\r\n";

//   WiFiClient client;
//   HTTPClient http;

//   camera_fb_t *fb = esp_camera_fb_get();
//   if (!fb) {
//       Serial.println("Camera capture failed");
//       return;
//   }

//   // Create the HTTP payload
//   http.begin(client, serverUrl);
//   http.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

//   int httpResponseCode = http.POST(payload + String((char *)fb->buf, fb->len) + "\r\n--" + boundary + "--\r\n");

//   if (httpResponseCode > 0) {
//       Serial.printf("HTTP Response code: %d\n", httpResponseCode);
//       String response = http.getString();
//       Serial.println("Server response: " + response);
      
//   } else {
//       Serial.printf("HTTP POST failed: %s\n", http.errorToString(httpResponseCode).c_str());
//   }

//   esp_camera_fb_return(fb);
//   http.end();
// }
#endif // FACE_UTILS_H