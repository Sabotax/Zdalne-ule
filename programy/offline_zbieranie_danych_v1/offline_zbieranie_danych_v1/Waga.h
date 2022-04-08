/*
 * Klasa Waga, zbiera dane o wadze z hx711
 */
#pragma once
#include "HX711.h"

class Waga {
  public:
    const int pin_data = 2;
    const int pin_clk = 3;
    boolean DEBUG = false;
    HX711 scale;
    float calibration_factor = 1;
    boolean initiated_correctly = false;
    float measured_waga;

    boolean init();
    float measure();

    Waga() : scale(pin_data,pin_clk) {};
};
