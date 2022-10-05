#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "CGA2121_Tu7GnYu";
const char* password = "mtyRdz7KZcEc9k2Ezw";

const char* serverName = "http://daniel.rozycki.student.put.poznan.pl/incomingData.php";

const char* espMasterId = "esp01";

void initWifi() {
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

String SlaveDataShotToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp) {
  String re="[{\"espSlaveId\": \"";
  re += espSlaveId;

  re += "\"},{\"waga\": \"";
  re += String(waga) + "\"},{\"timestamp\": \"";

  re += myTimestamp + "\"},{\"temperatura_wewn\": \"";

  re += String(temperatura) + "\"}]";
  return re;
}

String WholeDataToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp,const float& temperatura_zewn) {
  String re = "";
  re += "{";
  re += "\"espMasterId\": \"" + String(espMasterId) + "\",";
  re += "\"temperatura_zewn\": \"" + String(temperatura_zewn) + "\",";
  re += "\"dane\": " + SlaveDataShotToJson(espSlaveId,waga,temperatura,myTimestamp);
  re += "}";
  return re;
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
      String httpRequestData = WholeDataToJson(espSlaveId,waga,temperatura,myTimestamp,69.0);

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
