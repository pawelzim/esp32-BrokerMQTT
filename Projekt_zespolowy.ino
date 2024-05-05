#include "MqttHandler.hpp"

// mqttHandler(ssid, password, mqtt_server, mqtt_user, mqtt_password)
//                                          ^puste ""  ^puste ""
MqttHandler mqttHandler(ssid, password, mqtt_server, mqtt_user, mqtt_password);

void setup() {
    mqttHandler.begin();
}

void loop() {
    mqttHandler.handle();
}



