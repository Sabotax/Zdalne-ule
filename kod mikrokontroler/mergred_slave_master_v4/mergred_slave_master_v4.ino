/*
 * Zadania programu:
 * - inicjacja:
 *     - RTC: 
 *          - jeśli first uruchomienie z kompa to ustawia godzine
 *     - SD
 *     - HX711
 *     - WIFI lub GSM w zależności od opcji
 * - wake up cykl:
 *     - pobiera wage
 *     - zapisuje na SD wraz z timestampem
 *     - wysyła na serwer przez WiFi
 *     - idzie spać, cykl na nowo się zaczyna
 * - dodatkowa opcja BLE:
 *     - obudzenie/włączenie przełącznikiem fizycznym, które sprawia że esp chodzi tak długo jak jest przełączone (sygnalizowane ledem)
 *     - w tym czasie esp wystawia serwer/usługę, podczas której będzie można synchronizować dane z smartfonem przez bluetooth
 *     
 * Kolejne wersje mają mieć: budzenie dodatkowo przez czujnik wstrząsu, gps
 */
#define DEBUG
#define GSM_turn_on
//#define WIFI_turn_on
#define BLE_turn_on
#define mockRTC
#define mockWeight
#define mockBattery
//#define turnOnSD
//#define wakeUpTouch
#define GsmMockOn
#define GsmPostMockOn

#include "authData.h"

#include "MyHelper.h"
#include "MyRTC.h"
#include "MySD.h"
#include "MyWaga.h"

#ifdef GSM_turn_on
  #include "MyGSM.h"
#else
  #include "MyWIFI.h"
#endif

#ifdef BLE_turn_on
  #include "MyBLE.h"
  #include "MyBLECommsController.h"
#endif

void setup() {
  setCpuFrequencyMhz(80); //dla hx711
  Serial.begin(115200);
  SerialAT.begin(38400, SERIAL_8N1, RX2, TX2);

  delay(3000);

  //print_wakeup_touchpad();
  //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

  #ifdef BLE_turn_on
    initBLE();
  #endif

  #ifndef mockRTC
    initMyRTC();
  #endif

  #ifndef mockWeight
    initMyWaga();
  #endif

  #ifdef turnOnSD
    initMySD();
  #endif

//todo
//  #ifdef GSM_turn_on
//    wakeUpGSM();
//  #endif

  #ifdef GSM_turn_on
    //while(!initGSM()) {};
  #endif
  #ifdef WIFI_turn_on
    initMyWIFI();
  #endif

  initTimer();
}

void loop() {
 
  //disconnecting
  if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      #ifdef DEBUG
        Serial.println(F("BLE restart advertising"));
      #endif
      oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
  }

  if(czasZapisu) {
    // todo, na koniec ostatniej operacji z zapisu, zdjecie flagi
    //czasZapisu = false;

    uint32_t isrCount = 0, isrTime = 0;
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
    
    #ifdef DEBUG
    Serial.print("onTimer no. ");
    Serial.print(isrCount);
    Serial.print(" at ");
    Serial.print(isrTime);
    Serial.println(" ms");
    #endif

    #ifdef mockWeight
      wagaOdczyt = random(100);
    #else
      wagaOdczyt = loadcell.get_units(2);
    #endif

    #ifdef mockRTC
      nowTimestampEpoch = "1";
    #else
      nowTimestampEpoch = getEpoch();
    #endif

    #ifdef turnOnSD
      saveDataToSD(SD, dataToCsvRow(wagaOdczyt, nowTimestampEpoch) );
    #endif

    sendingData = true;
    czasZapisu = false;
  }

  if(sendingData && !deviceConnected) {
    #ifdef GSM_turn_on
      if ( getBattery() ) {
        if(!dataToSendCreated) {
          dataToSend = dataToJson(wagaOdczyt, nowTimestampEpoch,String(batteryMeasure));
          dataToSendCreated = true;
        }
        makePostGSM();
      }
    #endif
    #ifdef WIFI_turn_on
      sendDataToServer( dataToJson(wagaOdczyt, nowTimestampEpoch,batteryPercent) );
      sendingData = false;
    #endif
  }

  //obsluga komunikacji
  executeComms();
}
