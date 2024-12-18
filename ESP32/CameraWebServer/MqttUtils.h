#pragma once
#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H
#include "config.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <base64.h>
#include "esp_camera.h"
#include "UtilsWifi.h"

// Callback function for received MQTT messages
void callback(char* topic, byte* payload, unsigned int length);

void mqtt_connect();

#endif // MQTT_UTILS_H