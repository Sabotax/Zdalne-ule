#include <LowPower.h>

/*
 * Zbadane rzeczy:
 * Jeśli serial jest przed powerdown, to trzeba dać czas aby serial zdążył wysłac dane np:
 *    Serial.println("Ide spac" + String(i));
      delay(10);
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Setup - start");
}
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Ide spac" + String(i));
  delay(10);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  i++;
  Serial.println("Obudzilem sie" + String(i) );
}
