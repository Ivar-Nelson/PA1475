#include <RH_ASK.h>
#include <SPI.h>
#include <Arduino.h>
#include <WiFi.h>

const char *SSID = "Filip";
const char *PWD = "qbzg9370";

RH_ASK driver(2000, 16, 12, 0);
short int old_message = 1;

char *int_to_string(short int num)
{
    short int tmpnum = num;
    int len = 1;
    while (tmpnum > 9)
    {
        tmpnum /= 10;
        len++;
    }
    char* retstr = (char*)calloc(len+1, sizeof(char));
    char backwards[len];
    for (int i = 0; i < len; i++, num /= 10)
    {
        backwards[i] = num % 10 + '0';
    }
    for(int i = 0; i < len; i++){
        retstr[i] = backwards[len-1-i];
    }
    retstr[len] = 0;
    return retstr;
}

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
char *mqttServer = "192.168.145.95";
int mqttPort = 1883;

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
//  mqttClient.setCallback(callback);
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

void setup() {
  Serial.println("Started");
  connectToWiFi();
  setupMQTT();
  Serial.begin(115200);
  pinMode(16, INPUT);
  if(!driver.init()) Serial.println("error: did not init");
  Serial.println("Setup complete");
}

void loop() {
  mqttClient.loop();
  driver.setModeRx();
  if (!mqttClient.connected())
    reconnect();
  uint8_t buf[2];
  uint8_t len = 2;
  driver.recv(buf, &len);
  uint16_t message = *(uint16_t*)buf; 
  // message = 49;
  if(message != 0){
    char* pubstr = int_to_string(message);
    Serial.print("Received: ");
    Serial.print(pubstr); 
    Serial.print("\n");
    mqttClient.publish("esp32/light_sensor_test", pubstr);
  }
}
