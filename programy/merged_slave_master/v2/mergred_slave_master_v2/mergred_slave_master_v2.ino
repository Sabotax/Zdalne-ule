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
//#define GSM_turn_on
#define BLE_turn_on

#include "MyHelper.h"
#include "MyRTC.h"
#include "MySD.h"
#include "MyWaga.h"

#ifdef BLE_turn_on
  #include "MyBLE.h"
#endif

#ifdef GSM_turn_on
  #include "MyGSM.h"
#else
  #include "MyWIFI.h"
#endif

// TODO osobny plik z stałymi wartościami nie-na-repo typu ssid, auth, serwer itp

void setup() {
  setCpuFrequencyMhz(80); //dla hx711
  Serial.begin(115200);

  enableTouchWakeUp();

  didIwakeUpForBle();
  //print_wakeup_touchpad();
  //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

  //initMyRTC();
  //initMySD();
//  initMyWaga();

//  #ifdef GSM_turn_on
//    initMyGSM();
//  #else
//    initMyWIFI();
//  #endif

//  #ifdef BLE_turn_on
//    initBLE(); // póki co zawsze włącza, nie tylko po przełączeniu fizycznym
//  #endif
}

void loop() {
  //float wagaOdczyt = loadcell.get_units(2);

  //String nowTimestamp = getTimestamp();

  //saveDataToSD(SD, dataToCsvRow(wagaOdczyt, nowTimestamp) );

//  #ifdef GSM_turn_on
//    connectGSM();
//    makePostGSM( dataToJson(wagaOdczyt, nowTimestamp) );
//  #else
//    sendDataToServer( dataToJson(wagaOdczyt, nowTimestamp) );
//  #endif

  handle_sleep();
}
