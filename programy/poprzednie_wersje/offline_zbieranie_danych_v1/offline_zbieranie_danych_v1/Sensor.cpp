#include "Sensor.h"

boolean Sensor::all_initiated_correctly() {
  return Temperatura1.initiated_correctly && Waga1.initiated_correctly;
}

void Sensor::init() {
  Temperatura1.init();
  Waga1.init();
}
