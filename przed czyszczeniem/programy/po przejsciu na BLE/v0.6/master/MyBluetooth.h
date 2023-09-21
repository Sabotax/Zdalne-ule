#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_sensory = NULL;
BLECharacteristic* pCharacteristic_spanie = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t czas_snu[2] = {0,30};

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// The remote service we wish to connect to.
#define serviceUUID "a9a5d9db-1d47-44a6-84cb-a3d00cf2a25f"
// The characteristic of the remote service we are interested in.
#define charUUID_spanie "453a0b55-0417-4ee6-b458-83d3ff6fe054"
#define charUUID_sensory "2aedd564-e21d-491d-a34a-6aaf984c4e3c"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }

};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      #ifdef DEBUG
      Serial.println("BLE callback on " + String(pCharacteristic->toString().c_str()));
      Serial.println(F("Received:"));
      if (value.length() > 0) {
        for (byte i = 0; i < value.length(); i++)
          Serial.print(value[i]);
      }
      #endif

      pCharacteristic_spanie->setValue((uint8_t*) &czas_snu, 2);
      pCharacteristic_spanie->notify();

      // todo get data from value

      //sendDataToServer("espSlave01", 69.11, 25.3, getTimestamp() );
      //readFile(SD, "/foo.txt");
      //writeFile(SD, "/hello.txt", "Hello ");
      saveDataToSD(SD,"espSlave01", 69.11, 25.3, getTimestamp() );
    }
};

void initBluetooth() {
  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(serviceUUID);

  // Create a BLE Characteristic
  pCharacteristic_sensory = pService->createCharacteristic(
                      charUUID_sensory,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic_spanie = pService->createCharacteristic(
    charUUID_spanie,
    BLECharacteristic::PROPERTY_READ   |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
  );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic_sensory->addDescriptor(new BLE2902());
  pCharacteristic_spanie->addDescriptor(new BLE2902());

  pCharacteristic_sensory->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceUUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println(F("Waiting a client connection to notify..."));
}
