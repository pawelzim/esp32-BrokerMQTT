#include "time.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "MqttHandler.hpp"

MqttHandler::MqttHandler(const char* Ssid, const char* Password, const char* Mqtt_server, const char* Mqtt_user, const char* Mqtt_password) 
    : ssid(Ssid), password(Password), mqtt_server(Mqtt_server), mqtt_user(Mqtt_user), mqtt_password(Mqtt_password) {}

void MqttHandler::begin() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.connect("ESP32Client", mqtt_user, mqtt_password);
    pinMode(2, OUTPUT);
}

void MqttHandler::handle() {
    if (!client.connected()) {
        digitalWrite(2, LOW);
        reconnect();
    }
    digitalWrite(2, HIGH);
    client.loop();

    StaticJsonDocument<200> doc;
    doc["carId"] = "QWE123123RTY";
    doc["isTurnedOn"] = true;
    doc["velocity"] = 100.0;
    doc["fuelLevel"] = 20.0;
    // nie dzialaja te te funkcje, zapewne kwestia przypisania do tego jsona
    // doc["time"] = getTime();
    // doc["date"] = getDate();

    // char jsonBuffer[512];
    // serializeJson(doc, jsonBuffer);
    // client.publish("topic/testing/qwerty", jsonBuffer);
    Serial.println("Published topic");

    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
}

void MqttHandler::setup_wifi() {
    delay(10);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Attempting Wifi connection to: ");
        Serial.println(ssid);
    }

    Serial.println("");
    Serial.print("Connected to WiFi: ");
    Serial.println(ssid);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void MqttHandler::reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection to: ");
        Serial.println(mqtt_server);
        if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");

            delay(5000);
        }
    }
}

void MqttHandler::setupTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain NTP time.");
    } else {
        Serial.println("NTP time set successfully");
    }
}

String MqttHandler::getTime() {
    static char timeStr[9];
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        strcpy(timeStr, "Error");
        return String(timeStr);
    }
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    return String(timeStr);
}

String MqttHandler::getDate() {
    static char dateStr[11];
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        strcpy(dateStr, "Error");
        return String(dateStr);
    }
    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &timeinfo);
    return String(dateStr);
}
