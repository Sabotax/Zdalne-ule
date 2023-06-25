#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_RX_ESP = NULL;
BLECharacteristic* pCharacteristic_TX_ESP = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define serviceUUID_ESP "a9a5d9db-1d47-44a6-84cb-a3d00cf2a25f"
// The characteristic of the remote service we are interested in.
#define charUUID_RX_ESP "453a0b55-0417-4ee6-b458-83d3ff6fe054"
#define charUUID_TX_ESP "ff80d3e7-31ef-4d0c-b9a2-ffe499188e9f"

long timer_BLE = 0;

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
      Serial.println("ktos mi zapisal na RX");
      MyRX();
      //R gdy telefon coś zapisze na tą charakterystykę to tutaj to odebrać
      std::string value = pCharacteristic->getValue();

      Serial.println("BLE callback on " + String(pCharacteristic->toString().c_str()));
      Serial.println(F("Received:"));
      if (value.length() > 0) {
        for (byte i = 0; i < value.length(); i++)
          Serial.print(value[i]);
      }
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
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic_TX_ESP = pService->createCharacteristic(
    charUUID_TX_ESP,
    BLECharacteristic::PROPERTY_READ   |
    BLECharacteristic::PROPERTY_NOTIFY 
  );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic_RX_ESP->addDescriptor(new BLE2902());
  pCharacteristic_TX_ESP->addDescriptor(new BLE2902());

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

}

void MyTX(String s) {
  Serial.println("Setting new characteristic value to \"" + s + "\"");
  
  // Set the characteristic's value to be the array of bytes that is actually a string.
  pCharacteristic_TX_ESP->setValue(std::string(s.c_str()));
  pCharacteristic_TX_ESP->notify();
}
