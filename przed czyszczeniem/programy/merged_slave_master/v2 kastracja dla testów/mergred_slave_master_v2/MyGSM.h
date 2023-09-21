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
#define DUMP_AT_COMMANDS
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
  //modem.poweroff();
  //modem.sleepEnable();
}
void wakeUpGSM() {
  SerialMon.println("powering GSM");
  Serial2.begin(38400, SERIAL_8N1, RX2, TX2);
  Serial2.println("AT");
  modem.restart();
}
void initMyGSM() {
  Serial2.begin(38400, SERIAL_8N1, RX2, TX2);
  Serial2.println("AT");
  delay(6000);

  SerialMon.println("Initializing modem...");
  modem.restart();
  delay(6000);
  Serial2.println("AT");

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
    //return;
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
void ShowSerialData()
{
  while (Serial2.available() != 0)
    Serial.write(Serial2.read());
  delay(1000);
}
void sendAT(String s) {
  Serial.println("Komenda: " + s);
  Serial2.println(s);
}
void myComm(String sendtoserver) {
  if (Serial2.available())
  Serial.write(Serial2.read());
 
  sendAT("AT");
  delay(3000);
 
  sendAT("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
 
  sendAT("AT+HTTPPARA=\"CID\",1");
  delay(6000);
  ShowSerialData();
 
  sendAT("AT+HTTPPARA=\"URL\",\"http://lepotato-zdalneule.ddns.net/incomingData.php\""); //Server address
  delay(4000);
  ShowSerialData();
 
  sendAT("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(4000);
  ShowSerialData();
 
 
  sendAT("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
  //Serial.println(sendtoserver);
  delay(6000);
  ShowSerialData();
 
  sendAT(sendtoserver);
  delay(6000);
  ShowSerialData;
 
  sendAT("AT+HTTPACTION=1");
  delay(6000);
  ShowSerialData();
 
  sendAT("AT+HTTPREAD");
  delay(6000);
  ShowSerialData();
 
  sendAT("AT+HTTPTERM");
  delay(10000);
  ShowSerialData;
}
// TODO wywalić całe tinyGSM, zrobic wlasne uzytkowanie sima, dać na httpINIT, że jak jest error to robi restart i jeszcze raz próbuje, i tak 2 razy, dodać dekodowanie odpowiedzi
void makePostGSM2(const String& dane) {
  SerialMon.println("making POST request with body:");
  SerialMon.println(dane);

  myComm(dane);
  
//  String contentType = "application/json";
//  http.post(serverAndResource, contentType, dane);
//  int statusCode = http.responseStatusCode();
//  String response = http.responseBody();
//
//  //if(statusCode != 200) reportBug(statusCode);

//  modem.sendAT("");
//  String x;
//  modem.waitResponse(1000L,x);
//  SerialMon.println("Odp na test to |" + x + "|");
  /*
   * 21:00:31.411 -> AT
21:00:31.411 -> 
21:00:31.411 -> OK
21:00:31.411 -> Odp na test to |
21:00:31.411 -> OK
21:00:31.411 -> |
21:00:31.411 -> AT+HTTPINIT

   */
  
//  modem.sendAT("+HTTPINIT");
//  modem.waitResponse();
//  modem.sendAT("+HTTPPARA=\"CID\",1");
//  modem.waitResponse();
//  modem.sendAT("+HTTPPARA=\"URL\",\"http://lepotato-zdalneule.ddns.net/incomingData.php\"");
//  modem.waitResponse();
//  modem.sendAT("+HTTPPARA=\"CONTENT\",\"application/json\"");
//  modem.waitResponse();
//  
//  modem.sendAT("+HTTPDATA=" + String(dane.length()) + ",100000");
//  modem.waitResponse();
//  modem.streamWrite(dane);
//  modem.stream.flush();
//
//  SerialAT.println("AT+HTTPACTION=1");
//  delay(6000);
//  while (SerialAT.available() != 0)
//    Serial.write(SerialAT.read());
//  delay(1000);
//  
//  
////  modem.sendAT("+HTTPACTION=1");
////  delay(7000);
////  String resAction="";
////  while(modem.stream.available() != 0)
////    resAction += (char) modem.stream.read();
////  SerialMon.println("Odp na action to |" + resAction + "|");
//
//  SerialAT.println("AT+HTTPREAD");
//  delay(6000);
//  while (SerialAT.available() != 0)
//    Serial.write(SerialAT.read());
//  delay(1000);
//
////  modem.sendAT("+HTTPREAD");
////  delay(7000);
////  String resRead="";
////  while(modem.stream.available() != 0)
////    resRead += (char) modem.stream.read();
////  SerialMon.println("Odp na read to |" + resRead + "|");
//
//  modem.sendAT("+HTTPTERM");
//  modem.waitResponse();
}

String myGetBattery() {
  uint8_t  chargeState = -99;
  int8_t   percent     = -99;
  uint16_t milliVolts  = -9999;
  modem.getBattStats(chargeState, percent, milliVolts);
  return String(percent);
}
