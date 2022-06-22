// WIFI SLAVE
#include <WiFi.h>
#include <HTTPClient.h>
#define UPDATE_TIME 500
String nom = "Slave0";
const char* ssid = "CGA2121_Tu7GnYu";
const char* password = "mtyRdz7KZcEc9k2Ezw";
String command;
unsigned long previousRequest = 0;
WiFiClient master;
IPAddress server(192, 168, 0, 1);

// INNE ZMIENNE
boolean DEBUG = true;
boolean setup_error = false;
boolean loop_error = false;
unsigned int sleep_time = 40;


// POZOSTALE FUNKCJE

void setup() {
  Serial.begin(115200);
  if (DEBUG) Serial.println("Start programu");

  // WIFI
  IPAddress localIp(192,168,0,150);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(localIp,gateway,subnet);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEBUG) Serial.print(F("."));
  }
  if (DEBUG) Serial.print(nom);
  if (DEBUG) Serial.print(F(" connected to Wifi! IP address : ")); 
  if (DEBUG) Serial.println(WiFi.localIP());

//  // SEND
//  if (DEBUG) Serial.println("Proba polaczenia z serwerem");
//  if (master.connect(server, 80)) { // Connection to the server
//    master.println("GET /OdbiorDanych?ID=0&waga="+String(pomiar_waga)+"&godzina="+String(est_seconds)+"&temp="+String(tempC)+" /HTTP/1.1");
//    //answer
//    if (DEBUG) Serial.println("Wyslano wiadomosc na serwer");
//    if (DEBUG) Serial.flush();
//    master.flush();
//  }
//  else {
//    if (DEBUG) Serial.println("Nie udalo sie polaczyc z serwerem");
//  }

  // new edit
  HTTPClient http;
  // configure traged server and url
  Serial.print("[HTTP] begin...\n");
  http.begin("http://example.com/index.html");
  
  // start connection and send HTTP header
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(5000);

}

void loop() {
}
