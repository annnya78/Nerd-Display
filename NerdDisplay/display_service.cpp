#include "display_service.h"
#include "app.h"
#include <MD_Parola.h>

struct EffItem { const char* name; textEffect_t eff; };

template<typename T, size_t N>
static textEffect_t effFromName(const T (&arr)[N], const String& n, textEffect_t fallback) {
  String u = n; u.trim(); u.toUpperCase();
  for (size_t i = 0; i < N; i++) if (u == arr[i].name) return arr[i].eff;
  return fallback;
}
template<typename T, size_t N>
static String nameFromEff(const T (&arr)[N], textEffect_t e, const char* fallback = "NO_EFFECT") {
  for (size_t i = 0; i < N; i++) if (arr[i].eff == e) return String(arr[i].name);
  return String(fallback);
}

// Effekte (bewusst hier zusammen mit den Helpern)
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

static textEffect_t effectIn()  { return effFromName(EFFECTS_IN,  App::params.effect_in,  PA_SCROLL_LEFT); }
static textEffect_t effectOut() { return effFromName(EFFECTS_OUT, App::params.effect_out, PA_SCROLL_LEFT); }

namespace Display {
  void begin() {
    App::matrix.begin();
    applyParams();
    App::matrix.displayClear();
  }

  void applyParams() { App::matrix.setIntensity(App::params.brightness); }

  void showImmediate(const String& s, uint32_t dwellMs) {
    App::matrix.displayText(s.c_str(), PA_CENTER, App::params.speed, dwellMs, PA_PRINT, PA_NO_EFFECT);
    App::matrix.displayReset();
    App::matrix.displayAnimate();
  }

  void startWith(const String& s) {
    App::matrix.displayText(s.c_str(), PA_CENTER, App::params.speed, App::params.dwell, effectIn(), effectOut());
  }

  void nextMessage() {
    if (App::params.messages.empty()) return;
    startWith(App::params.messages[App::msgIndex]);
    App::msgIndex = (App::msgIndex + 1) % App::params.messages.size();
  }

  void startInfoScroll() {
    String combined = WiFi.localIP().toString() + ", " + App::mdnsHost + ".local, ";
    static char infoText[128];
    combined.toCharArray(infoText, sizeof(infoText));
    App::matrix.displayText(infoText, PA_CENTER, 40, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
    App::matrix.displayReset();
  }

  bool animateOnce() {
    return App::matrix.displayAnimate();
  }
}
