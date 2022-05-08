/*
 * Spis pinów
 * 
 * GPIO11 - OneWire Dallas temperatura
 * 
 * GPIO7 - HX711 data
 * GPIO6 - HX711 clk
 * 
 * Karta pamięci
 * MISO D6
 * MOSI D7
 * SCK D5
 * CS D8
 * 
 */
 // https://everythingesp.com/make-your-state-variables-survive-unexpected-reboots/


// WIFI SLAVE
#include <ESP8266WiFi.h>
#define UPDATE_TIME 500
String nom = "Slave0";
const char* ssid = "CGA2121_Tu7GnYu";
const char* password = "mtyRdz7KZcEc9k2Ezw";
String command;
unsigned long previousRequest = 0;
WiFiClient master;
IPAddress server(192, 168, 0, 44);

// INNE ZMIENNE
boolean DEBUG = true;
boolean setup_error = false;
boolean loop_error = false;
unsigned int sleep_time = 20;
int i=0;

// POZOSTALE FUNKCJE

void setup() {
  Serial.begin(115200);
  if (DEBUG) Serial.println("Start programu");

  // WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEBUG) Serial.print(F("."));
  }
  if (DEBUG) Serial.print(nom);
  if (DEBUG) Serial.print(F(" connected to Wifi! IP address : ")); 
  if (DEBUG) Serial.println(WiFi.localIP());
 

  // SLEEP
  //ESP.deepSleep(sleep_time * 1e6 - millis() * 1e3);
}

void loop() {
  // WIFI
  if (master.connect(server, 80)) { // Connection to the server
    master.println(nom + ": " + String(i) +'\r');
    //answer
    if (DEBUG) Serial.println("Wyslano wiadomosc na serwer");
    if (DEBUG) Serial.flush();
    master.flush();
  }
  delay(5000);
}
