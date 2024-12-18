#include "MqttUtils.h"

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Control LED based on message (example)
  if (strcmp(topic, faceTopic) == 0) {
    faceServerHandle(message);
  }
}

void mqtt_connect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ArduinoClient", mqtt_user, mqtt_password)) { // Use a unique client ID
      Serial.println("connected");
      // Subscribe to topics
      // mqttClient.subscribe(ledTopic);
      // mqttClient.subscribe(messageTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}