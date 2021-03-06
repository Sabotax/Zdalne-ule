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
String nazwa = "dataT.txt";
String text = "";

// LowPower
#include <LowPower.h>
int czas_snu = 64;
long teraz;


// INNE ZMIENNE
boolean DEBUG = false;
boolean setup_error = false;
boolean loop_error = false;

// POZOSTALE FUNKCJE

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
    setup_error = true;
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

  pinMode(LED_BUILTIN,OUTPUT);
  while(setup_error) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}

void loop() {
  for(int i = 0; i < czas_snu/8;i++) {
    LowPower.powerDown(SLEEP_8S,ADC_OFF, BOD_OFF);
  }

  loop_error = false;

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

  // RTC POMIAR
  now = rtc.now();
  if (DEBUG) Serial.println("Teraz jest czas: " + toString(now));

  // KARTA SD ZAPIS
  myFile = SD.open(nazwa, FILE_WRITE);

  if (myFile) {
    if (DEBUG) Serial.print("Writing to " + nazwa);
    //text = toString(now) + "," + String(tempC) + "," + String(pomiar_waga);
    myFile.print(toString(now)+",");
    myFile.flush();
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
  // WAIT FOR ALL TASKS DONE (SAVING)
  //teraz = millis();
  delay(100);
  //while teraz 
  // ERROR LOOP
  int i = 0;
  while(loop_error && i < 100) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    i++;
  }
}
