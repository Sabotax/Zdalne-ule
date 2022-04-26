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

// INNE ZMIENNE
boolean DEBUG = true;
boolean setup_error = false;
boolean loop_error = false;

// POZOSTALE FUNKCJE

void setup() {
  Serial.begin(115200);
  Serial.println("Start programu");

  // TEMPERATURA
  sensors.begin();
  if(DEBUG) Serial.println("Temperatura zainicjalizowana pomyslnie");

  // WAGA
  scale.set_scale(calibration_factor);
  scale.tare();
  long zero_factor = scale.read_average();

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
    myFile.println(String(pomiar_waga));
    myFile.flush();
    myFile.close();
    if (DEBUG) Serial.println("done.");
  } else {
    if (DEBUG) Serial.println("error opening " + nazwa);
    loop_error = true;
  }

  if (DEBUG) Serial.println();
  if (DEBUG) Serial.flush();

  // SLEEP
  //ESP.deepSleep(60e6);
  delay(10e3);
}

void loop() {
}
