#include "display_service.h"
#include "app.h"

#include <ESP8266WiFi.h>
#include <MD_Parola.h>

// ========================
// Effekt-Helfer
// ========================
struct EffItem { const char* name; textEffect_t eff; };

template<typename T, size_t N>
static textEffect_t effFromName(const T (&arr)[N], const String& n, textEffect_t fallback) {
  String u = n; u.trim(); u.toUpperCase();
  for (size_t i = 0; i < N; i++) if (u == arr[i].name) return arr[i].eff;
  return fallback;
}

// Effekt-Tabellen
static const EffItem EFFECTS_IN[] PROGMEM = {
  {"PRINT",          PA_PRINT},
  {"SCROLL_DOWN",    PA_SCROLL_DOWN},
  {"SCROLL_UP",      PA_SCROLL_UP},
  {"SCROLL_LEFT",    PA_SCROLL_LEFT},
  {"SCROLL_RIGHT",   PA_SCROLL_RIGHT},
  {"OPENING",        PA_OPENING},
  {"OPENING_CURSOR", PA_OPENING_CURSOR},
};
static const EffItem EFFECTS_OUT[] PROGMEM = {
  {"NO_EFFECT",       PA_NO_EFFECT},
  {"SCROLL_DOWN",     PA_SCROLL_DOWN},
  {"SCROLL_UP",       PA_SCROLL_UP},
  {"SCROLL_LEFT",     PA_SCROLL_LEFT},
  {"SCROLL_RIGHT",    PA_SCROLL_RIGHT},
  {"CLOSING",         PA_CLOSING},
  {"CLOSING_CURSOR",  PA_CLOSING_CURSOR},
};

static textEffect_t effectFromNameIn (const String& n)  { return effFromName(EFFECTS_IN,  n, PA_SCROLL_LEFT); }
static textEffect_t effectFromNameOut(const String& n)  { return effFromName(EFFECTS_OUT, n, PA_SCROLL_LEFT); }

// ========================
// Display-API
// ========================
namespace Display {

  void begin() {
    App::matrix.begin();
    applyParams();
    App::matrix.displayClear();
  }

  void applyParams() {
    App::matrix.setIntensity(App::params.brightness);
  }

  void showImmediate(const String& s, uint32_t dwellMs) {
    // MD_Parola erwartet pause als uint16_t; clampen
    uint16_t pause = (dwellMs > 65535U) ? 65535U : (uint16_t)dwellMs;
    App::matrix.displayText(s.c_str(), PA_CENTER, App::params.speed, pause, PA_PRINT, PA_NO_EFFECT);
    App::matrix.displayReset();
    App::matrix.displayAnimate();
  }

  // Variante ohne Effekte: globale Defaults + globaler dwell
  void startWith(const String& s) {
    uint16_t pause = (App::params.dwell > 65535U) ? 65535U : (uint16_t)App::params.dwell;
    App::matrix.displayText(s.c_str(), PA_CENTER, App::params.speed, pause,
                            effectFromNameIn(App::params.effect_in),
                            effectFromNameOut(App::params.effect_out));
  }

  // Variante mit per-Message Effekten, aber globalem dwell
  void startWith(const String& s, const String& effInName, const String& effOutName) {
    const String inName  = effInName.length()  ? effInName  : App::params.effect_in;
    const String outName = effOutName.length() ? effOutName : App::params.effect_out;
    uint16_t pause = (App::params.dwell > 65535U) ? 65535U : (uint16_t)App::params.dwell;
    App::matrix.displayText(s.c_str(), PA_CENTER, App::params.speed, pause,
                            effectFromNameIn(inName),
                            effectFromNameOut(outName));
  }

  // NEU: per-Message Effekte + per-Message dwell (Override)
  void startWith(const String& s, const String& effInName, const String& effOutName, int32_t dwellOverrideMs) {
    const String inName  = effInName.length()  ? effInName  : App::params.effect_in;
    const String outName = effOutName.length() ? effOutName : App::params.effect_out;

    uint32_t base = App::params.dwell;
    uint32_t chosen = (dwellOverrideMs >= 0) ? (uint32_t)dwellOverrideMs : base;
    uint16_t pause = (chosen > 65535U) ? 65535U : (uint16_t)chosen;

    App::matrix.displayText(s.c_str(), PA_CENTER, App::params.speed, pause,
                            effectFromNameIn(inName),
                            effectFromNameOut(outName));
  }

  void nextMessage() {
    if (App::params.messages.empty()) return;
    const MessageItem& m = App::params.messages[App::msgIndex];
    startWith(m.text, m.eff_in, m.eff_out, m.dwell_ms);  // nutzt Override falls gesetzt
    App::msgIndex = (App::msgIndex + 1) % App::params.messages.size();
  }

  void startInfoScroll() {
    const String combined = WiFi.localIP().toString() + ", " + App::mdnsHost + ".local, ";
    static char infoText[128];
    combined.toCharArray(infoText, sizeof(infoText));
    App::matrix.displayText(infoText, PA_CENTER, 40, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
    App::matrix.displayReset();
  }

  bool animateOnce() {
    return App::matrix.displayAnimate();
  }
}
