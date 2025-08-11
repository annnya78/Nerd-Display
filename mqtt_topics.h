#pragma once
#include "app.h"

namespace Topics {
  inline String set (const String& key)  { return App::baseTopic + "set/"   + key; }
  inline String state(const String& key) { return App::baseTopic + "state/" + key; }
  inline String meta(const String& key)  { return App::baseTopic + "meta/"  + key; }
}
