/*
 * Zadania programu:
 * - inicjacja:
 *     - RTC: 
 *          - jeśli first uruchomienie z kompa to ustawia godzine
 *     - SD
 *     - HX711
 *     - WIFI (zamiast GSM tymczasowo)
 * - wake up cykl:
 *     - pobiera wage
 *     - zapisuje na SD wraz z timestampem
 *     - wysyła na serwer przez WiFi
 *     - idzie spać, cykl na nowo się zaczyna
 * 
 * Kolejne wersje mają mieć: GSM zamiast wifi, budzenie dodatkowo przez czujnik wstrząsu
 */
#define DEBUG

// WAGA
#include <HX711.h>
#define DOUT  32
#define CLK  33 
HX711 loadcell;

//RTC
#include "RTClib.h"
#include <Wire.h>
RTC_DS3231 rtc;

//SD
#include "FS.h"
#include "SD.h"
#include "SPI.h"
char bufor[32768];
uint16_t bufor_index = 0;

//WIFI
#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = /*"CGA2121_Tu7GnYu"*/ "NETIASPOT-C0D740";
const char* password = /*"mtyRdz7KZcEc9k2Ezw"*/ "9533f5tdu5av";
const char* serverName = "http://daniel.rozycki.student.put.poznan.pl/incomingData.php";

//SLEEP
#define TIME_TO_SLEEP  10
#define uS_TO_S_FACTOR 1000000

// funkcje
void handle_sleep() {
  Serial.println("Czas spania: " + String(TIME_TO_SLEEP * uS_TO_S_FACTOR) + " sekund" );
  Serial.flush();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
String getTimestamp() {
  DateTime now = rtc.now();
  String re = "";
  re += String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + "T";
  re += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  return re;
}

String getDay() {
  DateTime now = rtc.now();
  String re = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day());
  return re;
}
const char* espMasterId = "esp01";

String SlaveDataShotToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp) {
  String re="[{\"S\": \""; //espSlaveId
  re += espSlaveId;

  re += "\",\"W\": \""; //waga
  re += String(waga) + "\"";
  //timestamp
  re += ",\"I\": \"";
  //temp wewn
  re += String(temperatura) + "\"}]";

  return re;
}
void saveDataToSD(fs::FS &fs,const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp) {
  String filename = "/"+getDay()+".json";
  Serial.println("filename: "+filename);
  File file = fs.open(filename, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    //reportBug(29);
    return;
  }
  else {
    String message = SlaveDataShotToJson(espSlaveId,waga,temperatura,myTimestamp)+";";
    if(file.print(message)){
      Serial.println("Message appended - saveDataToSD");
    } else {
      Serial.println("Append failed");
      //reportBug(30);
    }
    file.close();
  }
}
void sendDataToServer(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp ) {
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      #ifdef DEBUG
      Serial.println(F("Begin http send"));
      #endif
      
      http.begin(client, serverName);

      http.addHeader("Content-Type", "application/json");

//      SlaveDataShot ob1("espSlave1",5.69,25.3);
//      SlaveDataShot ob2("espSlave1",7.88,20.6);
//      SlaveDataShot arr[2] = {ob1, ob2};
      String httpRequestData = SlaveDataShotToJson(espSlaveId,waga,temperatura,myTimestamp);

      #ifdef DEBUG
      Serial.println(F("httpRequestData:"));
      Serial.println(httpRequestData);
      Serial.println(F("\n sending to server"));
      #endif

      int httpResponseCode = http.POST(httpRequestData);
      #ifdef DEBUG
      Serial.println(F("HTTP Response code: "));
      Serial.println(httpResponseCode);
      #endif
      // Free resources
      http.end();
    }
    else {
      #ifdef DEBUG
      Serial.println(F("WiFi Disconnected"));
      #endif
    }
}

void setup() {
  setCpuFrequencyMhz(80);
  Serial.begin(115200);

  //WAGA
  loadcell.begin(DOUT, CLK);
  loadcell.set_scale();
  loadcell.set_offset();

  //RTC
  if (! rtc.begin()) {
    #ifdef DEBUG
      Serial.println("Couldnt find RTC");
    #endif
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    #ifdef DEBUG
    Serial.println("RTC lost power, let's set the time!");
    #endif
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //SD
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      //reportBug(27);
      return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      //reportBug(28);
      return;
  }

  //WIFI
  WiFi.begin(ssid, password);
  #ifdef DEBUG
  Serial.println(F("WIFI Connecting"));
  #endif
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.print(".");
    #endif
  }
  #ifdef DEBUG
  Serial.println();
  Serial.print(F("Connected to WiFi network with IP Address: "));
  Serial.println(WiFi.localIP());
  #endif

}

void loop() {
  float wagaOdczyt = loadcell.get_units(2);

  String nowTimestamp = getTimestamp();

  saveDataToSD(SD,"espMasterId", wagaOdczyt, 69.3, nowTimestamp );
  sendDataToServer("espMasterId", wagaOdczyt, 69.3, nowTimestamp );

  handle_sleep();
}
