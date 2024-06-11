#include "MqttHandler.hpp"

const char* deviceName = "device001";

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "";
const char* mqtt_login = "";
const char* mqtt_password = "";

// mqttHandler(ssid, password, mqtt_server, mqtt_user, mqtt_password)
//                                          ^puste ""  ^puste ""
MqttHandler mqttHandler(deviceName, ssid, password, mqtt_server, mqtt_login, mqtt_password);

void setup() {
    mqttHandler.begin();
}

void loop() {
    // mqttHandler.handle(speed, latitude, longitude, direction, 
    //                    isIgnitionOn, isDriving, isDrivingNeutral,
    //                    fuelLevel, mileage, diagnosticCodes);
    mqttHandler.handle();
}