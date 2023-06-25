#include "MyBLE.h"

void setup() {
  setCpuFrequencyMhz(80); //dla hx711
  Serial.begin(115200);

  initBLE();
}

void loop() {
  if (deviceConnected && timer_BLE + 5000 < millis() ) {
      timer_BLE = millis();
      #ifdef DEBUG
        Serial.println(F("client connected")); 
      #endif
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        #ifdef DEBUG
          Serial.println(F("BLE start advertising"));
        #endif
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  MyTX(String(random(300)));
  delay(5000);
}
