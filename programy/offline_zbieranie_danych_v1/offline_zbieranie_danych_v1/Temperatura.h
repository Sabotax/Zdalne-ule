/* Klasa sensor
 *  Zbiera dane dotyczące wagi i temperatury
 */
#pragma once
#include <OneWire.h>

class Temperatura {
  public:
    boolean DEBUG = false;
    const int pin_temperatura = 10;
    OneWire ds;
    byte data[12];
    byte addr[8];
    byte type_s;

    boolean init();
    float measure();

    Temperatura() : ds(pin_temperatura) {};
};
