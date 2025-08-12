#include "app.h"

namespace App {
  MD_Parola matrix(HARDWARE_TYPE, PIN_CS, MAX_DEVICES);
  ESP8266WebServer server(80);
  WiFiClient net;
  PubSubClient mqtt(net);

  PersistentConfig cfg;
  RuntimeParams params;

  String deviceId;
  String baseTopic;
  String mdnsHost;

  volatile bool setBootstrapped = false;
  volatile bool hasInitFlag     = false;

  bool ipScrollMode = false;
  uint8_t msgIndex = 0;
  unsigned long lastMqttReconnect = 0;

  void setRuntimeDefaults() {
    params.brightness = 3;
    params.speed      = 60;
    params.dwell      = 3000;
    params.effect_in  = "SCROLL_LEFT";
    params.effect_out = "SCROLL_LEFT";
    params.messages   = { { "Nerd-Display", "", "" } }; // text, eff_in, eff_out
  }
}
