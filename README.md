# Nerd-Display (ESP8266 + MD_Parola + MQTT + WebUI)

Zeigt Texte/Werte auf einer 4x MAX7219 (FC16) Matrix. Konfiguration per WebUI (LittleFS) und MQTT.

## Features
- MD_Parola Effekte (in/out)
- MQTT: `set/*` (Kommandos), `state/*` (retained), `meta/*`
- Bootstrapping: Defaults nach erstem Connect
- WebUI (LittleFS) für Konfiguration

## Ordnerstruktur
NerdDisplay.ino
app.h/.cpp
config_store.h/.cpp
display_service.h/.cpp
mqtt_service.h/.cpp
mqtt_topics.h
web_handlers.h/.cpp
webui.h

## Abhängige Libraries
- ESP8266 Core
- MD_Parola, MD_MAX72XX
- PubSubClient
- ArduinoJson
- WiFiManager
- LittleFS

## MQTT Beispiele
- Einzelner Text:
  - Topic: `<baseTopic>/set/text`
  - Payload: `"Hallo"` oder `{"text":"Hallo"}`
- Mehrere Texte:
  - Payload: `["Hallo","Zebra"]` oder `{"messages":["Hallo","Zebra"]}`

## Lizenz
MIT (siehe LICENSE)