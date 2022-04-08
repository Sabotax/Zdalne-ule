/* Klasa sensor
 *  Zbiera dane dotyczące wagi i temperatury
 */
#pragma once
#include <OneWire.h>
// TODO przebudować z użyciem przykadu simple może, (bilbioteka dallas temperatura)
class Temperatura {
  public:
    boolean DEBUG = false;
    const int pin_temperatura = 9;
    OneWire ds;
    byte data[12];
    byte addr[8];
    byte type_s;
    boolean initiated_correctly = false;
    float measured_temperature;

    boolean init();
    float measure();

    Temperatura() : ds(pin_temperatura) {};
};
