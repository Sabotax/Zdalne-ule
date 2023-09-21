#include <LowPower.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Setup - start");
}
int i = 0;
void loop() {
  spij(3);
  Serial.println("Obudzilem sie | " + String(millis()) );
}

void spij(int x) {
  Serial.println("Ide spac - start na 8*3 sekund |" + String(millis()));
  delay(10);
  for (int i=0; i < x ; i++) {
    Serial.println("Spie, i=" + String(i) + " | " + String(millis()));
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  }
}
