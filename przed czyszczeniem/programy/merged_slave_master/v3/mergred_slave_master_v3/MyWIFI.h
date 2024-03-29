//WIFI
#include <WiFi.h>
#include <HTTPClient.h>

void sendDataToServer(const String& dane) {
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      #ifdef DEBUG
      Serial.println(F("Begin http send"));
      #endif
      
      http.begin(client, serverName);

      http.addHeader("Content-Type", "application/json");

      #ifdef DEBUG
      Serial.println(F("httpRequestData:"));
      Serial.println(dane);
      Serial.println(F("\n sending to server"));
      #endif

      int httpResponseCode = http.POST(dane);
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

void initMyWIFI() {
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
