#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include <WiFi.h>
#include <PubSubClient.h>

extern const char* mqtt_server; // e.g., "192.168.1.10" or "broker.example.com"
extern const int mqtt_port;
extern const char* mqtt_user;
extern const char* mqtt_password;
extern const char* faceServerUrl;
// MQTT Topics
extern const char* faceTopic; // Topic to receive general messages
extern const char* faceSuccessTopic; // Topic to receive general messages

extern WiFiClient espClient;
extern PubSubClient mqttClient;

extern HardwareSerial SerialPort;
#endif