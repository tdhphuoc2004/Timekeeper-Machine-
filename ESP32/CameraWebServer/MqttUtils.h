#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H
#include <WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "192.168.1.14"; // e.g., "192.168.1.10" or "broker.example.com"
const int mqtt_port = 1883;
const char* mqtt_user = "user";
const char* mqtt_password = "123456";

// MQTT Topics
const char* faceTopic = "arduino/face"; // Topic to receive general messages

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void faceServerHandle(String message);

// Callback function for received MQTT messages
void callback(char* topic, byte* payload, unsigned int length);

void mqtt_connect();

#endif // MQTT_UTILS_H