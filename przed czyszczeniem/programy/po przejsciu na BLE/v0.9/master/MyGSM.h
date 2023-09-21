

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

// Server details
const char server[]   = "http://daniel.rozycki.student.put.poznan.pl";
const char resource[] = "/incomingData.php";
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

//void goSleepGSM() {
//  SerialMon.println("GSM cutting power");
//  modem.poweroff();
//}
//void wakeUpGSM() {
//  SerialMon.println("powering GSM");
//  modem.restart();
//}
void initGSM() {
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial2.println("AT");
  delay(6000);

  SerialMon.println("Initializing modem...");
  modem.restart();

  //SerialMon.println("Initializing modem...");
  //goSleepGSM();
}

void connectGSM() {
  modem.gprsConnect(apn, gprsUser, gprsPass);
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
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

  if (modem.isGprsConnected()) { SerialMon.println("GPRS connected"); }

  SerialMon.print("Connecting to ");
  SerialMon.println(server);
  if (!client.connect(server, port)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
}

void makePostGSM(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp) {
  //String postData = WholeDataToJson(espSlaveId,waga,temperatura,myTimestamp,69.0);
  //String postData = "{\"name\":\"Alice\",\"age\"=\"12\"}";
  String postData = "{\"T\": \"ExToken\",\"ID\": \"esp01\",\"ID2\": \"espA\",\"masa\": \"1.02\",\"t\": \"21.1\"}";
  SerialMon.println("making POST request with body:");
  SerialMon.println(postData);
  String contentType = "application/json";
  http.post("http://daniel.rozycki.student.put.poznan.pl/incomingData.php", contentType, postData);
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  //if(statusCode != 200) reportBug(statusCode);

  SerialMon.print("Status code: ");
  SerialMon.println(statusCode);
  SerialMon.print("Response: ");
  SerialMon.println(response);
  
}
