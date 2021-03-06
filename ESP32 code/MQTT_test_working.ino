#include <Arduino.h>
#include <WiFi.h>
const char *SSID = "Fille";
const char *PWD = "fillemanet";

void connectToWiFi() {
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
  
}

#include <PubSubClient.h>
// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "192.168.8.152";
int mqttPort = 1883;

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
//  mqttClient.setCallback(callback);
}

void setup() {
  connectToWiFi();
  setupMQTT();
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
      }
      
  }
}


void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  mqttClient.publish("esp32/light_sensor", "data");
  delay(1000);
  }
