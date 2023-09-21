// WIFI 
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
// JSON
#include <Arduino_JSON.h>

// INNE ZMIENNE
#define DEBUG true

// POZOSTALE FUNKCJE

void setup() {
  #if DEBUG
    Serial.begin(115200);
    Serial.println("Start programu");
  #endif

  // WIFI
  IPAddress localIp(192,168,1,150);
  IPAddress gateway(192,168,1,254);
  IPAddress subnet(255,255,255,0);
  WiFi.config(localIp,gateway,subnet);
  WiFi.begin(ssid,password);
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

//  // new edit
//  HTTPClient http;
//  // configure traged server and url
//  #if DEBUG
//    Serial.print("[HTTP] begin...\n");
//  #endif
//  // todo ogarnac zeby wyslalo postem jakies dane tam (jesli sie da, jak nie to get)
//  http.begin("http://daniel.rozycki.student.put.poznan.pl/upload");
//  
//  // start connection and send HTTP header
//  #if DEBUG
//    Serial.print("[HTTP] GET...\n");
//  #endif
//  // start connection and send HTTP header
//  //int httpCode = http.GET();
//
////  if(httpCode > 0) {
////    // HTTP header has been send and Server response header has been handled
////    #if DEBUG
////      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
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
////  }
//    
//  http.end();
}

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10000;

const char* serverName = "http://192.168.1.5:80/SERWER_PHP/";

struct test_object_struct {
  int id = 0;
  int waga = 5;
  int temp = 20;
} test_object;

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
//      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//      // Data to send with HTTP POST
//      String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
//      // Send HTTP POST request
//      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      String data_to_send = "{\"client_api_key\":\"xxx\",\"tab\":[{\"id\":\"0\",\"waga\":\"10\",\"temperatura\":\"5.3\"},{\"id\":\"0\",\"waga\":\"0\",\"temperatura\":\"5.7\"},{\"id\":\"0\",\"waga\":\"20\",\"temperatura\":\"10.3\"}]}";
      int httpResponseCode = http.POST(data_to_send);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response: ");

      if(httpResponseCode>0){
        String response = http.getString();  //Get the response to the request
        Serial.print("HTTP Code: ");
        Serial.println(httpResponseCode);   //Print return code
        Serial.println("HTTP response content:");
        Serial.println(response);           //Print request answer
      }else{
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
