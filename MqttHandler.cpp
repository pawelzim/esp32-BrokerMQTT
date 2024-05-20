#include "MqttHandler.hpp"

MqttHandler::MqttHandler(const char* DeviceName, const char* Ssid, const char* Password, const char* Mqtt_server, const char* Mqtt_user, const char* Mqtt_password) 
    : deviceName(DeviceName), ssid(Ssid), password(Password), mqtt_server(Mqtt_server), mqtt_user(Mqtt_user), mqtt_password(Mqtt_password), client(espClient) {}

void MqttHandler::begin() {
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    setupWifi();
    sendPostRequest();
    setupMqtt();
    setupTime();
}

void MqttHandler::handle(int speed, float latitude, float longitude, 
                         const char* direction, bool isIgnitionOn, bool isDriving, 
                         bool isDrivingNeutral, float fuelLevel, float mileage, 
                         const char* diagnosticCodes) {
    checkConnectionStatus();
    client.loop();

    StaticJsonDocument<512> doc;
    JsonArray location = doc.createNestedArray("location");
    doc["speed"] = speed;
    location.add(latitude);
    location.add(longitude);
    doc["direction"] = direction;
    doc["isIgnitionOn"] = isIgnitionOn;
    doc["isDriving"] = isDriving;
    doc["isDrivingNeutral"] = isDrivingNeutral;
    doc["fuelLevel"] = fuelLevel;
    doc["mileage"] = mileage;
    doc["diagnosticCodes"] = diagnosticCodes;
    doc["time"] = getTime();
    doc["date"] = getDate();

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);
    client.publish(deviceName, jsonBuffer);
    Serial.print("Published to topic: '");
    Serial.print(deviceName);
    Serial.println("'");

    delay(1000);
}

void MqttHandler::setupWifi() {
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
    Serial.print(ssid);
    Serial.print(" | IP address: ");
    Serial.println(WiFi.localIP());
}

void MqttHandler::sendPostRequest() {
    WiFiClientSecure clientPostRequest;
    clientPostRequest.setInsecure();

    HTTPClient httpsPostRequest;
    String serverName = "https://localhost:7240/devices/";
    String url = serverName + deviceName;

    while (true) {
        if (httpsPostRequest.begin(clientPostRequest, url)) {
            httpsPostRequest.addHeader("Content-Type", "application/json");

            int httpResponseCode = httpsPostRequest.POST("");

            if (httpResponseCode > 0) {
                String response = httpsPostRequest.getString();
                Serial.println(httpResponseCode);
                Serial.println(response);
                if (httpResponseCode == 200) {
                    break;
                }
            } else {
                Serial.print("Error sending POST: ");
                Serial.println(httpResponseCode);
            }

            httpsPostRequest.end();
        } else {
            Serial.println("Unable to connect");
        }

        delay(3000);
    }
}


// void MqttHandler::sendPostRequest() {
//     WiFiClientSecure clientPostRequest;
//     clientPostRequest.setInsecure();

//     HTTPClient httpsPostRequest;
//     String serverName = "https://localhost:7240/devices/";
//     String url = serverName + deviceName;

//     if (httpsPostRequest.begin(clientPostRequest, url)) {
//         httpsPostRequest.addHeader("Content-Type", "application/json");

//         StaticJsonDocument<200> doc;
//         doc["status"] = "started";
//         doc["deviceName"] = deviceName;

//         String requestBody;
//         serializeJson(doc, requestBody);

//         int httpResponseCode = httpsPostRequest.POST(requestBody);

//         if (httpResponseCode > 0) {
//             String response = httpsPostRequest.getString();
//             Serial.println(httpResponseCode);
//             Serial.println(response);
//         } else {
//             Serial.print("Error sending POST: ");
//             Serial.println(httpResponseCode);
//         }

//         httpsPostRequest.end();
//     } else {
//         Serial.println("Unable to connect");
//     }
// }

void MqttHandler::setupMqtt() {
    client.setServer(mqtt_server, 1883);
    client.connect("ESP32Client", mqtt_user, mqtt_password);
    if (client.connected()) {
        Serial.print("Connected to MQTT Broker ");
        Serial.println(mqtt_server);
    }
    else {
        reconnect_mqtt();
    }
}

void MqttHandler::setupTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain NTP time.");
    } else {
        Serial.println("NTP time set successfully");
        Serial.println("");
    }
}

void MqttHandler::reconnect_wifi() {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    WiFi.begin(ssid, password);
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
        Serial.println("Attempting to reconnect Wifi");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Reconnected to WiFi: ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("");
    } else {
        Serial.println("Failed to reconnect to WiFi");
    }
}

void MqttHandler::reconnect_mqtt() {
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

void MqttHandler::checkConnectionStatus() {
    if (WiFi.status() != WL_CONNECTED) {
        reconnect_wifi();
    }
    if (!client.connected()) {
        digitalWrite(2, LOW);
        reconnect_mqtt();
    }
    digitalWrite(2, HIGH);
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
