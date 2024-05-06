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
//#define mockRTC
//#define mockWeight
//#define mockBattery
#define turnOnSD
//#define wakeUpTouch

#include "authData.h"

#include "MyHelper.h"
#include "MyRTC.h"
#include "MySD.h"
#include "MyWaga.h"

#ifdef BLE_turn_on
  #include "MyBLE.h"
#endif

//#ifdef GSM_turn_on
  #include "MyGSM.h"
//#else
  #include "MyWIFI.h"
//#endif

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

  //todo
//  #ifdef GSM_turn_on
//    initMyGSM();
//  #endif
  #ifdef WIFI_turn_on
    initMyWIFI();
  #endif
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
      if((int) millis() % 5 == 0) {
        #ifdef DEBUG
          Serial.println(F("Client connected"));
        #endif
      }
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
    // Print it
    Serial.print("onTimer no. ");
    Serial.print(isrCount);
    Serial.print(" at ");
    Serial.print(isrTime);
    Serial.println(" ms");

//    #ifdef GSM_turn_on
//      initMyGSM();
//    #endif
//    #ifdef WIFI_turn_on
//      initMyWIFI();
//    #endif

    #ifdef mockWeight
      wagaOdczyt = random(100);
    #else
      wagaOdczyt = loadcell.get_units(2);
    #endif

    #ifdef mockRTC
      String nowTimestampEpoch = "1";
    #else
      String nowTimestampEpoch = getEpoch();
    #endif

//    #ifdef mockBattery
//      String batteryPercent = String(random(100));
//    #else
//      String batteryPercent = String(getBattery());
//    #endif

    #ifdef turnOnSD
      saveDataToSD(SD, dataToCsvRow(wagaOdczyt, nowTimestampEpoch) );
    #endif

//    #ifdef GSM_turn_on
//      makePostGSM( dataToJson(wagaOdczyt, nowTimestampEpoch,batteryPercent), true );
//    #endif
//    #ifdef WIFI_turn_on
//      sendDataToServer( dataToJson(wagaOdczyt, nowTimestampEpoch,batteryPercent) );
//    #endif
  }

  //obsluga komunikacji
//  if(deviceConnected) {
//    if(input_received) {
//          Serial.println("Przetwarzam input");
//          input_received = false;
//  
//          //decode input
//  
//          String rozkaz = "";
//          bool rozkaz_done = false;
//  
//          String data_incoming = "";
//          
//          for(uint8_t i = 0; i < input_size; i++) {
//            //Serial.println("Char" + String(i) + "=( "+String(input_data[i])+" , "+String( (char) input_data[i] ) + " )");
//            if(!rozkaz_done) {
//              if(input_data[i] == '|') {
//                rozkaz_done = true;
//              }
//              else {
//                rozkaz += String( (char) input_data[i] );
//              }
//            }
//            else {
//              data_incoming += String( (char) input_data[i] );
//            }
//          }
//          // input decoded
//          Serial.println("rozkaz = " + rozkaz);
//          Serial.println("dane = " + data_incoming);
//  
//          #ifdef turnOnSD
//  
//            if(rozkaz == "1") {
//              // start sending data
//              // TODO dekodowanie path?
//              String path = "/"+data_incoming+".txt";
//              Serial.println("Reading file: " + path);
//    
//              file = SD.open(path);
//              if(!file){
//                Serial.println("Failed to open file for reading");
//                String path2 = data_incoming+".txt";
//                Serial.println("Reading file: " + path2);
//                file = SD.open(path);
//                Serial.println("drugie otwarcie:" + bool(file));
//                myTXstring(5,"0");
//              }
//              else {
//                myTXstring(1,"");
//              }
//              
//        
//              // TODO jak zrobić otwieranie pliku i wysyłanie pomiędzy kolejnymi pętlami
//              // deklaracja wyżej i otwieranie zamykanie w 1 i 3 ?
//            }
//    
//            if(rozkaz == "2") {
//              //continue sending data
//              Serial.println("file "+ String((bool) file));
//              if(file){
//                String line = "";
//                char c='#';
//                
//                while(file.available()){
//                  c = file.read();
//                  Serial.println(c);
//                
//                  if(c == '#') {
//                    // file ended
//      
//                    // TX with message(rozkaz) that file ended (and line)
//                    file.close();
//                    myTXrow(3,line);
//                    break;
//                  }
//                  else if(c == ';') {
//                    // line ended
//      
//                    // TX with line and rozkaz that is ready to continue
//                    myTXrow(2,line);
//                    break;
//                  }
//                  else {
//                    line += String(c);
//                  }
//                }
//    
//                if(!file.available()) {
//                  myTXstring(6,"");
//                  file.close();
//                }
//              }
//              
//            }
//  
//          #endif
//  
//          #ifndef mockWeight
//  
//            if(rozkaz == "4") {
//              // send current weight
//              wagaOdczyt = loadcell.get_units(2);
//              Serial.println("wykonuje pomiar="+String(wagaOdczyt));
//              myTXstring(4,String(wagaOdczyt));
//            }
//    
//            if(rozkaz == "7") {
//              // set offset to rtc and weight
//              weight_offset = data_incoming.toFloat();
//              loadcell.set_offset(weight_offset);
//              myTXstring(7,"");
//            }
//    
//            if(rozkaz == "8") {
//              // set scale to rtc and weight
//              weight_scale = data_incoming.toFloat();
//              loadcell.set_scale(weight_scale);
//              myTXstring(8,"");
//            }
//
//            if(rozkaz == "9") {
//              // tare
//              loadcell.tare();
//              myTXstring(9,"");
//            }
//          #endif
//  
//          if(rozkaz == "10") {
//            // TODO opcjonalnie set czas spania
//          }
//  
//          #ifdef GSM_turn_on
//  //TODO jesli zakladamy że gsm śpi poprawnie, to musimy je obudzić żeby zdobyć te dane, a następnie uśpić z powrotem
//            if(rozkaz == "11") {
//              // Read current battery
//              handshakeGSM();
//    
//              String batteryPercent = String(getBattery());
//              myTXstring(11,batteryPercent);
//            }
//    
//            if(rozkaz == "12") {
//              // Read current signal
//              handshakeGSM();
//              
//              int signalStrength = getSignal();
//              myTXstring(12,String(signalStrength));
//            }
//          #endif
//
//          if(rozkaz == "A") {
//            uint32_t converted_epoch = data_incoming.toInt();
//            setTimeRTC(converted_epoch);
//          }
//        }
//  }
}
