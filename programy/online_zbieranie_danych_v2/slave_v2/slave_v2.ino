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

// TEMPERATURA
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//--zmienne
float tempC;

// WAGA
#define pin_waga_data D1
#define pin_waga_clk D2
#include "HX711.h"
HX711 scale(pin_waga_data,pin_waga_clk);
float calibration_factor = 1;
//-- zmienne
float pomiar_waga;

// KARTA SD
#include <SPI.h>
#include <SD.h>
#define pin_CS D8
File myFile;
//--zmienne
String nazwa = "dataK.txt";
String text = "";

// built-in RTC memory
#include <RTCVars.h>
RTCVars state;
long est_seconds;
long reset_counter;

// WIFI SLAVE
#include <ESP8266WiFi.h>
#define UPDATE_TIME 500
String nom = "Slave0";
const char* ssid = "Esp-master";
const char* password = "";
String command;
unsigned long previousRequest = 0;
WiFiClient master;
IPAddress server(192, 168, 0, 44);

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
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEBUG) Serial.print(F("."));
  }
  if (DEBUG) Serial.print(nom);
  if (DEBUG) Serial.print(F(" connected to Wifi! IP address : ")); 
  if (DEBUG) Serial.println(WiFi.localIP());
 
  // TEMPERATURA
  sensors.begin();
  if(DEBUG) Serial.println("Temperatura zainicjalizowana pomyslnie");

  // WAGA
  scale.set_scale(calibration_factor);
  scale.tare();
  long zero_factor = scale.read_average();

  // RTC memory
  state.registerVar( &est_seconds );
  state.registerVar( &reset_counter);
  if (state.loadFromRTC()) {            // we load the values from rtc memory back into the registered variables
    reset_counter++;
    if (DEBUG) Serial.println("This is reset no. " + (String)reset_counter);
    state.saveToRTC();                  // since we changed a state relevant variable, we store the new values
  } else {
    reset_counter = 0;                  // cold boot part
    if (DEBUG) Serial.println("This seems to be a cold boot. We don't have a valid state on RTC memory");
    est_seconds = 0;
  }

  // KARTA SD
  if (DEBUG) Serial.print("Initializing SD card...");
  if( !SD.begin(pin_CS) ) {
    if (DEBUG) Serial.println("initialization failed!");
    setup_error = true;
  }
  else {
    if (DEBUG) Serial.println("initialization done.");
  }
  delay(1000);

  // --------POMIARY
  // TEMPERAUTRA - POMIAR
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    if (DEBUG) Serial.print("Temperature for the device 1 (index 0) is: ");
    if (DEBUG) Serial.println(tempC);
  } 
  else
  {
    if (DEBUG) Serial.println("Error: Could not read temperature data");
    loop_error = true;
  }

  // WAGA POMIAR
  pomiar_waga = scale.get_units();
  if (DEBUG) Serial.println("Odczyt wagi: " + String(pomiar_waga));

  // KARTA SD ZAPIS
  myFile = SD.open(nazwa, FILE_WRITE);

  if (myFile) {
    if (DEBUG) Serial.print("Writing to " + nazwa);
    myFile.print(String(tempC) + ",");
    myFile.flush();
    myFile.print(String(est_seconds) + ",");
    myFile.flush();
    myFile.println(String(pomiar_waga));
    myFile.flush();
    myFile.close();
    if (DEBUG) Serial.println("done.");
  } else {
    if (DEBUG) Serial.println("error opening " + nazwa);
    loop_error = true;
  }
  if (DEBUG) Serial.flush();

  // WIFI
  if (DEBUG) Serial.println("Proba polaczenia z serwerem");
  if (master.connect(server, 80)) { // Connection to the server
    master.println("GET /OdbiorDanych?ID=0&waga="+String(pomiar_waga)+"&godzina="+String(est_seconds)+"&temp="+String(tempC)+" /HTTP/1.1");
    //answer
    if (DEBUG) Serial.println("Wyslano wiadomosc na serwer");
    if (DEBUG) Serial.flush();
    master.flush();
  }
  else {
    if (DEBUG) Serial.println("Nie udalo sie polaczyc z serwerem");
  }

  // RTC memory
  est_seconds += sleep_time + millis() / 1e3;
  state.saveToRTC();
  // SLEEP
  ESP.deepSleep(sleep_time * 1e6 - millis() * 1e3);
}

void loop() {
}
