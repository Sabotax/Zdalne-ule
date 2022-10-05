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

//// TEMPERATURA
//#include <OneWire.h>
//#include <DallasTemperature.h>
//#define ONE_WIRE_BUS D4
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);
////--zmienne
//float tempC;
float tempC = 69;

//// WAGA
//#define pin_waga_data D1
//#define pin_waga_clk D2
//#include "HX711.h"
//HX711 scale(pin_waga_data,pin_waga_clk);
//float calibration_factor = 1;
////-- zmienne
//float pomiar_waga;
float pomiar_waga = 420;


// BLE SLAVE
#include "BLEDevice.h"
// The remote service we wish to connect to.
static BLEUUID serviceUUID("a9a5d9db-1d47-44a6-84cb-a3d00cf2a25f");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID_spanie("453a0b55-0417-4ee6-b458-83d3ff6fe054");
static BLEUUID charUUID_sensory("2aedd564-e21d-491d-a34a-6aaf984c4e3c");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic_spanie;
static BLERemoteCharacteristic* pRemoteCharacteristic_sensory;
static BLEAdvertisedDevice* myDevice;

// INNE ZMIENNE
boolean DEBUG = true;
boolean setup_error = false;
boolean loop_error = false;
unsigned int sleep_time = 40;

// SLEEP
#define TIME_TO_SLEEP  10
#define uS_TO_S_FACTOR 1000000


// POZOSTALE FUNKCJE
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.println("callback detected" + String(pBLERemoteCharacteristic->toString().c_str()) );
    uint8_t minutes = pData[0];
    uint8_t seconds = pData[1];
    Serial.println("Ide spac na " + String(pData[0]) + " minut i " + String(pData[1]) + " sekund");
    Serial.flush();
    handle_sleep(minutes,seconds);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic_spanie = pRemoteService->getCharacteristic(charUUID_spanie);
    if (pRemoteCharacteristic_spanie == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID_spanie.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    pRemoteCharacteristic_sensory = pRemoteService->getCharacteristic(charUUID_sensory);
    if (pRemoteCharacteristic_sensory == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID_sensory.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    if(pRemoteCharacteristic_spanie->canNotify())
      pRemoteCharacteristic_spanie->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void handle_sleep(uint8_t minutes, uint8_t seconds) {
  Serial.println("czas spania: " + String(minutes) + " minut i " + String(seconds) + " sekund" );
  Serial.flush();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  //esp_sleep_enable_timer_wakeup( (minutes*60+seconds) * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("ESP32 SLAVE");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

long timer = 0;

void loop() {
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  
    
  if (connected)  {
    if(timer + 10000 < millis() ) {
      timer = millis();
      String newValue = String(pomiar_waga)+","+String(tempC)+"," + String(millis()/1000);
      Serial.println("Setting new characteristic value to \"" + newValue + "\"");
      
      // Set the characteristic's value to be the array of bytes that is actually a string.
      pRemoteCharacteristic_sensory->writeValue(newValue.c_str(), newValue.length());
    }
    
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
}
