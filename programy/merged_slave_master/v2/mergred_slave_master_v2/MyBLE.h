#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_RX_ESP = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// The remote service we wish to connect to.
#define serviceUUID_ESP "a9a5d9db-1d47-44a6-84cb-a3d00cf2a25f"
// The characteristic of the remote service we are interested in.
#define charUUID_RX_ESP "453a0b55-0417-4ee6-b458-83d3ff6fe054"

// The remote service we wish to connect to.
static BLEUUID serviceUUID_TEL("cb6b0579-8d9d-4220-bdba-9e78c8a34b4c");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID_RX_TEL("ff80d3e7-31ef-4d0c-b9a2-ffe499188e9f");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic_RX_TEL;
static BLEAdvertisedDevice* myDevice;

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
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID_TEL);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID_TEL.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic_RX_TEL = pRemoteService->getCharacteristic(charUUID_RX_TEL);
    if (pRemoteCharacteristic_RX_TEL == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID_RX_TEL.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

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
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID_TEL)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

//------------

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }

};
void MyRX() {
  //R pamiętać, że tutaj jest wciąż callback (przerwanie), tylko lekkie operacje
}
class odbiorRX: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      MyRX();
      //R gdy telefon coś zapisze na tą charakterystykę to tutaj to odebrać
//      std::string value = pCharacteristic->getValue();
//      #ifdef DEBUG
//      Serial.println("BLE callback on " + String(pCharacteristic->toString().c_str()));
//      Serial.println(F("Received:"));
//      if (value.length() > 0) {
//        for (byte i = 0; i < value.length(); i++)
//          Serial.print(value[i]);
//      }
//      #endif
//
//      pCharacteristic_spanie->setValue((uint8_t*) &czas_snu, 2);
//      pCharacteristic_spanie->notify();
//
//      send_msg_flag = true;
    }
};

void initBLE() {
  // Create the BLE Device
  BLEDevice::init("ESP01");

  //R serwer

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(serviceUUID_ESP);

  // Create a BLE Characteristic
  pCharacteristic_RX_ESP = pService->createCharacteristic(
                      charUUID_RX_ESP,
                      BLECharacteristic::PROPERTY_WRITE // inne urządzenia mogą na tą charakterystykę tylko zapisywać
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic_RX_ESP->addDescriptor(new BLE2902());

  pCharacteristic_RX_ESP->setCallbacks(new odbiorRX());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceUUID_ESP);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println(F("Waiting a client connection to notify..."));

  //R klient
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void MyTX() {
  String newValue = "a";
  Serial.println("Setting new characteristic value to \"" + newValue + "\"");
  
  // Set the characteristic's value to be the array of bytes that is actually a string.
  pRemoteCharacteristic_RX_TEL->writeValue(newValue.c_str(), newValue.length());
}
