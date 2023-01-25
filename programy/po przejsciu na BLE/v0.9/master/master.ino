#define DEBUG

#include "MyHelper.h"
#include "MyRTC.h"
#include "MyGSM.h"
#include "MySDCard.h"
#include "MyBluetooth.h"

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  initRTC();
  initBluetooth();
  initSD();
  initGSM();
  connectGSM();
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

    if(send_msg_flag) {
      send_msg_flag = false;
      // todo get data from value
      //wakeUpGSM();
      //connectGSM();
      //sendDataToServer("espSlave01", 69.11, 25.3, getTimestamp() );
      //readFile(SD, "/foo.txt");
      //writeFile(SD, "/hello.txt", "Hello ");
      saveDataToSD(SD,"espSlave01", 69.11, 25.3, getTimestamp() );
      makePostGSM("espSlave01", 69.11, 25.3, getTimestamp());
      //delay(5000);
      //goSleepGSM();
    }
}
