/*
 * Spis pinów
 * ADC 5 - SCL (zegar rtc i2c , Wire)
 * ADC 4 - SDA (zegar rtc i2c , Wire)
 * 
 * DC 9 - OneWire Dallas temperatura
 * 
 * DC 4 - HX711 data
 * DC 5 - HX711 clk
 * 
 * Karta pamięci
 * MISO 12
 * MOSI 11
 * SCK 13
 * CS 10
 * 
 * Czujnik wstrząsu
 * 2
 */

#include "Sensor.h"
#include "Save.h"
#include "Timer.h"
Sensor Sensor1;
Save Save1;
Timer Timer1;
void setup() {
  // put your setup code here, to run once:
  delay(3000);
  Serial.begin(115200);
  Serial.println("Start programu");

  while(  ! (Sensor1.Waga1.initiated_correctly && Sensor1.Temperatura1.initiated_correctly && Save1.initiated_correctly && Timer1.initiated_correctly) ) {
    if(!Timer1.initiated_correctly) {
      Timer1.init();
    }
    if(!Save1.initiated_correctly) {
      Save1.init();
    }
    if(!Sensor1.Temperatura1.initiated_correctly) {
      Sensor1.Temperatura1.init();
    }
    if(!Sensor1.Waga1.initiated_correctly) {
      Sensor1.Waga1.init();
    }
  }
}

String krotka = "";
void loop() {

  Sensor1.Temperatura1.measured_temperature = Sensor1.Temperatura1.measure();
  Sensor1.Waga1.measured_waga = Sensor1.Waga1.measure();
  Timer1.now = Timer1.rtc.now();
  krotka = Timer1.print_DateTime(Timer1.now) + "," + Sensor1.Temperatura1.measured_temperature + "," + Sensor1.Waga1.measured_waga;
  Serial.println(krotka);
  Save1.save("data2.txt", krotka);
  delay(15);
  Timer1.spij_start(0,0,10);
}
