#pragma once
#include "Temperatura.h"
#include "Waga.h"

class Sensor {
  public:
    Temperatura Temperatura1;
    Waga Waga1;

    Sensor() : 
      Temperatura1(),
      Waga1()
      {};
};
