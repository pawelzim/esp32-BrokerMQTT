#include "MqttHandler.hpp"

const char* serverName = "https://localhost:7240/devices/";
const char* deviceName = "deviceId123";

const char* ssid = "Frida";
const char* password = "Frida27/72";

const char* mqtt_server = "192.168.0.183";
const char* mqtt_login = "";
const char* mqtt_password = "";

// mqttHandler(ssid, password, mqtt_server, mqtt_user, mqtt_password)
//                                          ^puste ""  ^puste ""
MqttHandler mqttHandler(deviceName, ssid, password, mqtt_server, mqtt_login, mqtt_password);
double tempValue = 150;

void setup() {
    mqttHandler.begin();
}

void loop() {
    // mqttHandler.handle(speed, latitude, longitude, direction, 
    //                    isIgnitionOn, isDriving, isDrivingNeutral,
    //                    fuelLevel, mileage, diagnosticCodes);
    mqttHandler.handle();
    tempValue += 15;
}