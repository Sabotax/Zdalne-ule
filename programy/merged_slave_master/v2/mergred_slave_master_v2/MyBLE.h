#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_RX_ESP = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

uint8_t input_data[512] = [];
uint8_t input_size = 0;
bool input_received = false;

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


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }

};
void MyRX(uint8_t* input) {
  //R pamiętać, że tutaj jest wciąż callback (przerwanie), tylko lekkie operacje
}
class odbiorRX: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      input_size = 0;
      for (uint8_t i = 0; i < value.length(); i++)
        input_data[i] = value[i];
        input_size++;
      }
    }
    input_received = true;
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
