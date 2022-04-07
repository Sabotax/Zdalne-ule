#include "Sensor.h"
#include "Save.h"
Sensor Sensor1;
Save Save1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Sensor1.Temperatura1.DEBUG = true;
  while( !Sensor1.Temperatura1.init() ) delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(Sensor1.Temperatura1.DEBUG);
  delay(1000);
}
