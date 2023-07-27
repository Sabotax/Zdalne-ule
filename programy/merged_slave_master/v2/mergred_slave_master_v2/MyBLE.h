#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_RX_ESP = NULL;
BLECharacteristic* pCharacteristic_TX_ESP = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

uint8_t input_data[512];
uint8_t input_size = 0;
bool input_received = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define serviceUUID_ESP "a9a5d9db-1d47-44a6-84cb-a3d00cf2a25f"
// The characteristic of the remote service we are interested in.
#define charUUID_RX_ESP "453a0b55-0417-4ee6-b458-83d3ff6fe054"
#define charUUID_TX_ESP "391150c1-8fe8-47f3-b045-9be85e40683b"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      file.close();
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
      for (uint8_t i = 0; i < value.length(); i++) {
        input_data[i] = value[i];
        input_size++;
      }
    }
    input_received = true;
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
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
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
}


void myTXstring(uint8_t rozkaz, String dataString) {
  uint8_t dane[dataString.length()+1];
  dane[0] = rozkaz;
  
  for(short i = 1 ; i < dataString.length()+1; i++) {
    dane[i] = (char) dataString[i-1];
  }
  
  pCharacteristic_TX_ESP->setValue(dane, dataString.length()+1);
  pCharacteristic_TX_ESP->notify();
}

void myTXrow(uint8_t rozkaz,String line) {

  // -- rozdzielenie na stringi
  String wagaStr = "";
  String epochStr = "";

  Serial.println("debug incoming line " + line);
  
  short lineSwitch = 0;
  for(short i = 0; i < line.length();i++) {
    if(line[i] == ',' || line[i] == '|') {
      Serial.println("debug switch " + String(lineSwitch));
      lineSwitch++;
      continue;
    }
    if(lineSwitch == 0){
      Serial.println("debug waga build " + String(line[i]));
      wagaStr += line[i];
    }
    if(lineSwitch == 1){
      Serial.println("debug epoch build " + String(line[i]));
      epochStr += line[i];
    }
  }
  
  // waga
  uint8_t calkowite=0;
  uint8_t ulamki=0;
  String calkowiteStr="";
  String ulamkiStr="";
  //waga < 255 musi byc
  bool calkowiteSwitch = true;
  for(short i = 0; i < wagaStr.length();i++) {
    if(wagaStr[i] == '.') {
      calkowiteSwitch = false;
      continue;
    }
    if(calkowiteSwitch){
      calkowiteStr += wagaStr[i];
    }
    if(!calkowiteSwitch){
      ulamkiStr += wagaStr[i];
    }
  }
  int calkowiteInt = atoi( calkowiteStr.c_str());
  int ulamkiInt = atoi( ulamkiStr.c_str());

  if(calkowiteInt > 255 || ulamkiInt > 255 || calkowiteInt < 0 || ulamkiInt < 0) {
    myTXstring(5,"1");
    return;
  }

  calkowite = (uint8_t) calkowiteInt;
  ulamki = (uint8_t) ulamkiInt;

  // czas
  Serial.println("debug epochStr przed konwersja" + epochStr);
  Serial.flush();
  uint32_t epochValue = (uint32_t) stoul(std::string(epochStr.c_str()));
  Serial.println("debug epochValue: " + String(epochValue));

  uint8_t epochValuePart0 = (epochValue & 0x000000ff);
  uint8_t epochValuePart1 = (epochValue & 0x0000ff00) >> 8;
  uint8_t epochValuePart2 = (epochValue & 0x00ff0000) >> 16;
  uint8_t epochValuePart3 = (epochValue & 0xff000000) >> 24;

  uint8_t tab[7] = {
    rozkaz,calkowite,ulamki,epochValuePart0,epochValuePart1,epochValuePart2,epochValuePart3
  };

  pCharacteristic_TX_ESP->setValue(tab,7);
  pCharacteristic_TX_ESP->notify();
}
