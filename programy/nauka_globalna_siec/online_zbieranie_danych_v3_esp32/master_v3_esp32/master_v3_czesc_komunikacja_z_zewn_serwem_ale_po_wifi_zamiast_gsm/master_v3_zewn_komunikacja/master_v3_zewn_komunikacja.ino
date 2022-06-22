// WIFI SLAVE
#include <WiFi.h>
#include <HTTPClient.h>
#define UPDATE_TIME 500

String nom = "ESP-MASTER-01";
#define dom true
#if dom
  const char* ssid = "NETIASPOT-C0D740";
  const char* password = "9533f5tdu5av";
#else
  const char* ssid = "CGA2121_Tu7GnYu";
  const char* password = "mtyRdz7KZcEc9k2Ezw";
#endif

// INNE ZMIENNE
#define DEBUG true

// POZOSTALE FUNKCJE

void setup() {
  #if DEBUG
    Serial.begin(115200);
    Serial.println("Start programu");
  #endif

  // WIFI
  IPAddress localIp(192,168,0,150);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(localIp,gateway,subnet);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #if DEBUG 
      Serial.print(F(".")); 
    #endif
  }
  #if DEBUG
    Serial.print(nom);
    Serial.print(F(" connected to Wifi! IP address : ")); 
    Serial.println(WiFi.localIP());
  #endif

  // new edit
  HTTPClient http;
  // configure traged server and url
  #if DEBUG
    Serial.print("[HTTP] begin...\n");
  #endif
  // todo ogarnac zeby wyslalo postem jakies dane tam (jesli sie da, jak nie to get)
  http.begin("http://daniel.rozycki.student.put.poznan.pl/upload");
  
  // start connection and send HTTP header
  #if DEBUG
    Serial.print("[HTTP] GET...\n");
  #endif
  // start connection and send HTTP header
  //int httpCode = http.GET();

//  if(httpCode > 0) {
//    // HTTP header has been send and Server response header has been handled
//    #if DEBUG
//      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//    #endif
//    
//    // file found at server
//    if(httpCode == HTTP_CODE_OK) {
//      String payload = http.getString();
//      #if DEBUG
//        Serial.println(payload);
//      #endif
//    }
//    
//  } 
//  else {
//    #if DEBUG
//      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//    #endif
//  }
    
  http.end();
}


void loop() {
}
