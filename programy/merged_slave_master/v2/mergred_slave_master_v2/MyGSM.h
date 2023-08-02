#define RX2 16
#define TX2 17

#define SerialMon Serial
#define SerialAT Serial2
#define TINY_GSM_MODEM_SIM800
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 9600

#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// Your GPRS credentials, if any
const char apn[]      = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

bool connectingSuccess = false;

#include <TinyGsmClient.h>
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif
#include <ArduinoHttpClient.h>
TinyGsmClient client(modem);
const int      port = 80;
HttpClient    http(client, server, port);

boolean send_msg_flag = false;

void goSleepGSM() {
  SerialMon.println("GSM cutting power");
  modem.poweroff();
}
void wakeUpGSM() {
  SerialMon.println("powering GSM");
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial2.println("AT");
  modem.restart();
}
void initMyGSM() {
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial2.println("AT");
  delay(6000);

  SerialMon.println("Initializing modem...");
  modem.restart();

  //SerialMon.println("Initializing modem...");
  //goSleepGSM();
}

void connectGSM() {
  long momentStartConnecting = millis();
  modem.gprsConnect(apn, gprsUser, gprsPass);
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork() || millis() > momentStartConnecting + (1000*60*1)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
  if (modem.isNetworkConnected()) { SerialMon.println("Network connected"); }

  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  if (modem.isGprsConnected()) { 
    SerialMon.println("GPRS connected"); 
    SerialMon.println("Signal strength: " + String(modem.getSignalQuality()));
  }

  SerialMon.print("Connecting to ");
  SerialMon.println(server);
  if (!client.connect(server, port)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
  connectingSuccess = true;
}

void makePostGSM(const String& dane) {
  SerialMon.println("making POST request with body:");
  SerialMon.println(dane);
  String contentType = "application/json";
  http.post(serverAndResource, contentType, dane);
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  //if(statusCode != 200) reportBug(statusCode);

  SerialMon.print("Status code: ");
  SerialMon.println(statusCode);
  SerialMon.print("Response: ");
  SerialMon.println(response);
  
}

String myGetBattery() {
  uint8_t  chargeState = -99;
  int8_t   percent     = -99;
  uint16_t milliVolts  = -9999;
  modem.getBattStats(chargeState, percent, milliVolts);
  return String(percent);
}
