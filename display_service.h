#pragma once
#include <Arduino.h>

namespace Display {
  void begin();                // App::matrix.begin + Grundsetup
  void applyParams();          // Helligkeit etc.
  void showImmediate(const String& s, uint32_t dwellMs = 0);
  void startWith(const String& s);
  void nextMessage();
  void startInfoScroll();      // IP + mDNS scrollen
  bool animateOnce();          // true, wenn eine Animation „fertig“ ist
}
