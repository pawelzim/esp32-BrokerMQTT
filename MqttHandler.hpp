#include "time.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

#ifndef MqttHandler_hpp
#define MqttHandler_hpp

class MqttHandler {
private:
    const char* ssid;
    const char* password;
    const char* mqtt_server;
    const char* mqtt_user;
    const char* mqtt_password;

    WiFiClient espClient;
    PubSubClient client;

    const long gmtOffset_sec = 3600;
    const int daylightOffset_sec = 3600;

    void setupTime();
    String getTime();
    String getDate();
    void reconnect();
    void setup_wifi();
public:
    MqttHandler(const char* Ssid, const char* Password, const char* Mqtt_server, const char* Mqtt_user, const char* Mqtt_password);
    void handle();
    void begin();
};

#endif
