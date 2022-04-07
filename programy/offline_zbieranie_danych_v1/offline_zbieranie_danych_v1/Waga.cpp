#include "Waga.h"

boolean Waga::init() {
  scale.set_scale(calibration_factor);
  scale.tare();
  long zero_factor = scale.read_average(); // po co?
  if (DEBUG) Serial.println("Waga zainicjalizowana");
  return true;
  // TODO sprawdzic jakos czy sie powiodlo i w zaleznosci zmienic return
}

float Waga::measure() {
  float x = 1;
  float odczyt = scale.get_units() * x;
  if (DEBUG) Serial.println("Odczyt wagi: " + String(odczyt));
  return odczyt;
}
