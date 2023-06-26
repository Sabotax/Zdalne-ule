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

  if(bleWakeUp) {
    if(millis() + (5*60*1000) > bleWakeUpMoment ) {
      // time's up, going to sleep
      handle_sleep();
    }
    else {
      if(input_received ) {
        input_received = false;

        //decode input

        String rozkaz = "";
        bool rozkaz_done = false;

        String data_incoming = "";
        
        for(uint8_t i = 0; i < input_size; i++) {
          if(!rozkaz_done) {
            if(input_data[i] == '|') {
              rozkaz_done = true;
            }
            else {
              rozkaz += String( (char) input_data[i] );
            }
          }
          else {
            data_incoming += input_data[i];
          }
        }
        // input decoded

        if(rozkaz == "1") {
          // start sending data

          //data incoming = który plik z datą brać
          //String line from file = "X"

          // TODO jak zrobić otwieranie pliku i wysyłanie pomiędzy kolejnymi pętlami
          // deklaracja wyżej i otwieranie zamykanie w 1 i 3 ?
        }

        if(rozkaz == "2") {
          //continue sending data
          
        }

        if(rozkaz == "3") {
          // close reading file
          
        }
        
      }
    }
  }
  else {
    handle_sleep();
  }

  
}
