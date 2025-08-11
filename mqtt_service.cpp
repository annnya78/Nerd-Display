#include "mqtt_service.h"
#include "app.h"
#include "mqtt_topics.h"
#include "display_service.h"
#include "config_store.h"
#include <ArduinoJson.h>

namespace {
  // Helpers
  inline void publishSetKey(const String &key, const String &val)   { App::mqtt.publish(Topics::set(key).c_str(),   val.c_str(), false); }
  inline void publishSetKey(const String &k, int v)                  { publishSetKey(k, String(v)); }
  inline void publishSetKey(const String &k, uint32_t v)             { publishSetKey(k, String(v)); }

  inline void publishStateKey(const String &key, const String &val)  { App::mqtt.publish(Topics::state(key).c_str(), val.c_str(), true); }
  inline void publishStateKey(const String &k, int v)                { publishStateKey(k, String(v)); }
  inline void publishStateKey(const String &k, uint32_t v)           { publishStateKey(k, String(v)); }

  inline void publishStateKeyQuoted(const String& key, const String& val) {
    String out = "\"" + val + "\"";
    App::mqtt.publish(Topics::state(key).c_str(), out.c_str(), true);
  }
  inline void ackEchoStateStr(const String &k, const String &v) { publishStateKeyQuoted(k, v); }

  void mqttCallback(char* topic, byte* payload, unsigned int length) {
    const String t(topic);
    String raw; raw.reserve(length + 1);
    for (unsigned int i = 0; i < length; i++) raw += (char)payload[i];
    raw.trim();

    // meta/set_bootstrapped
    if (t.endsWith("/meta/set_bootstrapped")) { App::setBootstrapped = (raw == "1"); return; }
    // legacy has_init
    if (t.endsWith("/has_init"))              { App::hasInitFlag = (raw == "1");      return; }

    // brightness
    if (t.endsWith("/set/brightness")) {
      if (raw.length() == 0) return;
      const int v = constrain(raw.toInt(), 0, 15);
      App::params.brightness = (uint8_t)v;
      Display::applyParams();
      publishStateKey("brightness", v);
      return;
    }
    // speed
    if (t.endsWith("/set/speed")) {
      if (raw.length() == 0) return;
      long v = raw.toInt();
      if (v <= 0) v = App::params.speed;
      App::params.speed = (uint16_t)constrain(v, 1, 65535);
      publishStateKey("speed", (int)App::params.speed);
      return;
    }
    // dwell
    if (t.endsWith("/set/dwell")) {
      if (raw.length() == 0) return;
      long v = raw.toInt();
      if (v < 0) v = App::params.dwell;
      App::params.dwell = (uint32_t)v;
      publishStateKey("dwell", (uint32_t)App::params.dwell);
      return;
    }
    // effect (legacy)
    if (t.endsWith("/set/effect")) {
      if (raw.length() == 0) return;
      String u = raw; u.toUpperCase();
      // einfacher: wir setzen beide direkt auf denselben Namen
      App::params.effect_in  = u;
      App::params.effect_out = u;
      ackEchoStateStr("effect_in",  App::params.effect_in);
      ackEchoStateStr("effect_out", App::params.effect_out);
      ackEchoStateStr("effect",     App::params.effect_in);
      return;
    }
    if (t.endsWith("/set/effect_in")) {
      if (raw.length() == 0) return;
      String u = raw; u.toUpperCase();
      App::params.effect_in = u;
      ackEchoStateStr("effect_in", App::params.effect_in);
      ackEchoStateStr("effect",    App::params.effect_in);
      return;
    }
    if (t.endsWith("/set/effect_out")) {
      if (raw.length() == 0) return;
      String u = raw; u.toUpperCase();
      App::params.effect_out = u;
      ackEchoStateStr("effect_out", App::params.effect_out);
      return;
    }
    // text
    if (t.endsWith("/set/text")) {
      if (raw.length() == 0) return;

      App::params.messages.clear();

      // Einmaliges JSON-Dokument für alle Varianten
      DynamicJsonDocument doc(768);
      DeserializationError err = deserializeJson(doc, raw);
      if (!err) {
        if (doc.is<JsonArray>()) {
          // Payload: ["Hallo","Zebra",...]
          for (JsonVariant v : doc.as<JsonArray>()) {
            String item = v.as<String>(); item.trim();
            if (item.length()) App::params.messages.push_back(item);
          }
        } else if (doc.is<JsonObject>() && doc["messages"].is<JsonArray>()) {
          // Payload: {"messages":["Hallo","Zebra"]}
          for (JsonVariant v : doc["messages"].as<JsonArray>()) {
            String item = v.as<String>(); item.trim();
            if (item.length()) App::params.messages.push_back(item);
          }
        } else if (doc.is<JsonObject>() && doc["text"].is<const char*>()) {
          // Payload: {"text":"Hallo"}
          String s = String((const char*)doc["text"]); s.trim();
          if (s.length()) App::params.messages.push_back(s);
        }
      }

      // Fallback: Plain-String (inkl. evtl. umschließender Anführungszeichen)
      if (App::params.messages.empty()) {
        String s = raw;
        if (s.length() >= 2 && s.startsWith("\"") && s.endsWith("\"")) {
          s = s.substring(1, s.length() - 1);
        }
        if (!s.length()) s = "Nerd-Display";
        App::params.messages.push_back(s);
      }

      // Animation neu starten
      App::msgIndex = 0;
      Display::nextMessage();

      // State-Ack (zurückmelden; wir spiegeln den ersten Eintrag)
      if (!App::params.messages.empty()) {
        String first = App::params.messages[0];
        String out = "\"" + first + "\"";
        App::mqtt.publish((Topics::state("text")).c_str(), out.c_str(), true);
      }
      return;
    }

    // sonst: ignorieren
  }
}

namespace Mqtt {
  void publishSetDefaultsRetained() {
    ::publishSetKey("brightness", (int)App::params.brightness);
    ::publishSetKey("speed",      (int)App::params.speed);
    ::publishSetKey("dwell",      (uint32_t)App::params.dwell);
    ::publishSetKey("effect_in",  App::params.effect_in);
    ::publishSetKey("effect_out", App::params.effect_out);
    ::publishSetKey("effect",     App::params.effect_in); // legacy
    const String txt = (App::params.messages.empty() ? "Nerd-Display" : App::params.messages[0]);
    ::publishSetKey("text", txt);
  }

  void publishFullStateSnapshot() {
    ::publishStateKey("brightness", (int)App::params.brightness);
    ::publishStateKey("speed",      (int)App::params.speed);
    ::publishStateKey("dwell",      (uint32_t)App::params.dwell);
    ::publishStateKeyQuoted("effect_in",  App::params.effect_in);
    ::publishStateKeyQuoted("effect_out", App::params.effect_out);
    ::publishStateKeyQuoted("effect",     App::params.effect_in); // legacy
    const String txt = (App::params.messages.empty() ? "Nerd-Display" : App::params.messages[0]);
    ::publishStateKeyQuoted("text", txt);
  }

  void publishInfo(const String& mode) {
    if (!App::mqtt.connected()) return;
    DynamicJsonDocument d(256);
    d["ip"]   = WiFi.localIP().toString();
    d["mdns"] = App::mdnsHost + ".local";
    d["mode"] = mode;
    String out; serializeJson(d, out);
    App::mqtt.publish((App::baseTopic + "info").c_str(), out.c_str(), true);
  }

  void begin() {
    App::mqtt.setBufferSize(4096);
    App::mqtt.setCallback(::mqttCallback);
  }

  bool reconnect() {
    if (App::cfg.mqttHost.length() == 0) return false;

    App::mqtt.setServer(App::cfg.mqttHost.c_str(), App::cfg.mqttPort);

    const String clientId = "NerdDisplay-" + App::deviceId;
    const bool ok = (App::cfg.mqttUser.length())
      ? App::mqtt.connect(clientId.c_str(), App::cfg.mqttUser.c_str(), App::cfg.mqttPass.c_str())
      : App::mqtt.connect(clientId.c_str());
    if (!ok) return false;

    // Bootstrap-Check
    App::setBootstrapped = false;
    const String metaTopic = Topics::meta("set_bootstrapped");
    App::mqtt.subscribe(metaTopic.c_str());
    unsigned long t0 = millis();
    while (millis() - t0 < 600) { App::mqtt.loop(); delay(10); }
    App::mqtt.unsubscribe(metaTopic.c_str());

    if (!App::setBootstrapped) {
      publishSetDefaultsRetained();
      App::mqtt.publish(metaTopic.c_str(), "1", true);
    }

    publishFullStateSnapshot();

    App::mqtt.subscribe((App::baseTopic + "set/#").c_str());

    publishInfo("ready");
    return true;
  }

  void loop() { App::mqtt.loop(); }
}
