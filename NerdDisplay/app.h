#pragma once
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <MD_Parola.h>
#include <vector>

struct PersistentConfig {
  String   mqttHost;
  uint16_t mqttPort = 1883;
  String   mqttUser;
  String   mqttPass;
  String   mqttBase;
  String   mdnsName;
};

struct RuntimeParams {
  uint8_t  brightness;
  uint16_t speed;
  uint32_t dwell;
  String   effect_in;
  String   effect_out;
  std::vector<String> messages;
};

namespace App {
  extern MD_Parola matrix;
  extern ESP8266WebServer server;
  extern WiFiClient net;
  extern PubSubClient mqtt;

  extern PersistentConfig cfg;
  extern RuntimeParams params;

  extern String deviceId;
  extern String baseTopic;
  extern String mdnsHost;

  extern volatile bool setBootstrapped;
  extern volatile bool hasInitFlag;

  extern bool ipScrollMode;
  extern uint8_t msgIndex;
  extern unsigned long lastMqttReconnect;

  void setRuntimeDefaults();
}
