#include "time.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "MqttHandler.hpp"

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
// mqtt_user i mqtt_password zostawic puste
const char* mqtt_user = "";
const char* mqtt_password = ""; 

MqttHandler mqttHandler(ssid, password, mqtt_server, mqtt_user, mqtt_password);

void setup() {
    mqttHandler.begin();
}

void loop() {
    mqttHandler.handle();
    delay(1000);
}



