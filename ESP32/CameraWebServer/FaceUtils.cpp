#include "FaceUtils.h"

// void send_image() {
//   camera_fb_t * fb = esp_camera_fb_get();
//   if (!fb) {
//     Serial.println("Camera capture failed");
//     return;
//   }

//   // Encode image to base64
//   String base64Image = base64::encode(fb->buf, fb->len); // Use base64 encoding function

//   // Publish image
//   Serial.println("Publishing image...");
//   client.publish(faceTopic, base64Image.c_str());

//   esp_camera_fb_return(fb);
}