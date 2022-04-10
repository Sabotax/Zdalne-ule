/*
 * Spis pinów
 * ADC 5 - SCL (zegar rtc i2c , Wire)
 * ADC 4 - SDA (zegar rtc i2c , Wire)
 * 
 * DC 9 - OneWire Dallas temperatura
 * 
 * DC 4 - HX711 data
 * DC 5 - HX711 clk
 * 
 * Karta pamięci
 * MISO 12
 * MOSI 11
 * SCK 13
 * CS 10
 * 
 * Czujnik wstrząsu
 * 2
 */

// TEMPERATURA
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//--zmienne
float tempC;

// WAGA
#define pin_waga_data 4
#define pin_waga_clk 5
#include "HX711.h"
HX711 scale(pin_waga_data,pin_waga_clk);
float calibration_factor = 1;
//-- zmienne
float pomiar_waga;

// ZEGAR RTC
#include "RTClib.h"
RTC_DS1307 rtc;
DateTime now;
//--zmienne
String data = "";
String godzina = "";

// WSTRZAS
#define pin_wstrzas 2

// KARTA SD
#include <SPI.h>
#include <SD.h>
#define pin_CS 10
File myFile;
//--zmienne
String nazwa = "data7.txt";
String text = "";

// LowPower
#include <LowPower.h>
int czas_snu = 16;


// INNE ZMIENNE
boolean DEBUG = true;

// POZOSTALE FUNKCJE

void reakcja_na_wstrzas() {
  if (DEBUG) Serial.println("Szturchnieto mnie");
  return;
}

String toString(const DateTime& ob) {
  String re = "";
  for( int i = 0; i < 6 ; i++) {
    switch(i) {
      case 0:
        re+=String(ob.year(),DEC);
        break;
      case 1:
        re+=String(ob.month(),DEC);
        break;
      case 2:
        re+=String(ob.day(),DEC);
        break;
      case 3:
        re+=String(ob.hour(),DEC);
        break;
      case 4:
        re+=String(ob.minute(),DEC);
        break;
      case 5:
        re+=String(ob.second(),DEC);
        break;
    }
    if(i != 5) re+=":";
  }
  return re;
}

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

  // RTC
  if ( !rtc.begin() ) {
    if(DEBUG) Serial.println("Couldn't find RTC");
  }
  else {
    //if (! rtc.isrunning()) {
    if( true) {
      if (DEBUG) Serial.println("RTC is NOT running, let's set the time!");
      now = DateTime(F(__DATE__), F(__TIME__));
      //now = DateTime(2022,4,10,19,2,0);
      if (DEBUG) Serial.println("Ustawiam czas na " + toString(now));
      rtc.adjust(now);
    }
    if (DEBUG) Serial.println("RTC initialized correctly");
  }

  // WSTRZAS
  pinMode(pin_wstrzas,INPUT);
  attachInterrupt(pin_wstrzas, reakcja_na_wstrzas, LOW);

  // KARTA SD
  if (DEBUG) Serial.print("Initializing SD card...");
  if( !SD.begin(pin_CS) ) {
    if (DEBUG) Serial.println("initialization failed!");
  }
  else {
    if (DEBUG) Serial.println("initialization done.");
  }
  
}

void loop() {
  for(int i = 0; i < czas_snu/8;i++) {
    LowPower.powerDown(SLEEP_8S,ADC_OFF, BOD_OFF);
  }

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
  }

  // WAGA POMIAR
  pomiar_waga = scale.get_units();
  if (DEBUG) Serial.println("Odczyt wagi: " + String(pomiar_waga));

  // RTC POMIAR
  now = rtc.now();
  if (DEBUG) Serial.println("Teraz jest czas: " + toString(now));

  // KARTA SD ZAPIS
  myFile = SD.open(nazwa, FILE_WRITE);

  if (myFile) {
    if (DEBUG) Serial.print("Writing to " + nazwa);
    text = toString(now) + "," + String(tempC) + "," + String(pomiar_waga);
    myFile.println(text);
    myFile.close();
    if (DEBUG) Serial.println("done.");
  } else {
    if (DEBUG) Serial.println("error opening " + nazwa);
  }

  if (DEBUG) Serial.println();
}
