#include <WiFi.h>
#include "time.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#ifndef MqttHandler_hpp
#define MqttHandler_hpp

class MqttHandler {
private:
    const char* ssid;
    const char* password;
    const char* mqtt_server;
    const char* mqtt_user;
    const char* mqtt_password;

    const char* deviceName;

    WiFiClient espClient;
    PubSubClient client;

    const long gmtOffset_sec = 3600;
    const int daylightOffset_sec = 3600;

    void setupWifi();
    void sendPostRequest();
    void setupMqtt();
    void setupTime();

    String getTime();
    String getDate();

    void checkConnectionStatus();
    void reconnect_wifi();
    void reconnect_mqtt();
public:
    MqttHandler(const char* DeviceName, const char* Ssid, const char* Password,
                const char* Mqtt_server, const char* Mqtt_user, const char* Mqtt_password);

    void handle(int speed=-1, float latitude=-1, float longitude=-1, const char* direction="error",
                bool isIgnitionOn=false, bool isDriving=false, bool isDrivingNeutral=false,
                float fuelLevel=-1, float mileage=-1, const char* diagnosticCodes="error");

    void begin();
};

#endif
