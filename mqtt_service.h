#pragma once
#include <Arduino.h>

namespace Mqtt {
  void begin();                 // nur Callback setzen, Buffergröße etc.
  bool reconnect();             // verbindet + bootstrapped + subscribes
  void loop();                  // mqtt.loop()

  void publishInfo(const String& mode);
  void publishFullStateSnapshot();
  void publishSetDefaultsRetained();
}
