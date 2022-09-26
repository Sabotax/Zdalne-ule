#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_sensory = NULL;
BLECharacteristic* pCharacteristic_spanie = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
#define N 32
uint8_t value[N] = {1,2,3,4};

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
      Serial.println("onWrite " + String(pCharacteristic->toString().c_str()));

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }

      pCharacteristic_spanie->setValue((uint8_t*)&value, N);
      pCharacteristic_spanie->notify();
    }
};



void setup() {
  Serial.begin(115200);

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
  Serial.println("Waiting a client connection to notify...");
}
long timer = 0;
void loop() {
    // notify changed value
    if (deviceConnected && timer + 5000 < millis() ) {
      timer = millis();
      Serial.println("client connected");
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
