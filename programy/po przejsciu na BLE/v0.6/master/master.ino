#define DEBUG

#include "MyRTC.h"
#include "MyWifi.h"
#include "MySDCard.h"
#include "MyBluetooth.h"

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  initRTC();
  initBluetooth();
  initWifi();
  initSD();
}
long timer = 0;
void loop() {
    // notify changed value
    if (deviceConnected && timer + 5000 < millis() ) {
      timer = millis();
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
}
