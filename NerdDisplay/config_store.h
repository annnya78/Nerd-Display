#pragma once
#include <Arduino.h>

namespace ConfigStore {
  extern const char* CONF_PATH;

  String macSuffix();
  String idFromMAC();

  String normalizedBase(String in);
  String makeBaseTopic();

  void ensurePersistentDefaults();
  void save();
  void load();
}
