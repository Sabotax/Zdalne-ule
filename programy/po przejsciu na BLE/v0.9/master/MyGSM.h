const char* serverName = "http://daniel.rozycki.student.put.poznan.pl/incomingData.php";
#include "TinyGSM.h"
#define RX2 16
#define TX2 17

#define SerialMon Serial
#define SerialAT Serial2
#define TINY_GSM_MODEM_SIM800
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 9600

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif

TinyGsmClient client(modem);
HttpClient    http(client, server, port);

#define BUFOR_GSM_SIZE 64
char buforGSM[BUFOR_GSM_SIZE];
byte buforGSM_used = 0;
const char* ending = "\r\nOK";

boolean awaitResponse() {
  boolean b = true;
  unsigned long start = millis();
  for(byte i=0;i<BUFOR_GSM_SIZE && b ;i++) {
    buforGSM[i] = (char) Serial.read();
    if(i>=5 && buforGSM[i-5] == '\\' && buforGSM[i-4] == 'r' && buforGSM[i-3] == '\\' && buforGSM[i-2] == 'n' && buforGSM[i-1] == 'O' && buforGSM[i] == 'K') {
      buforGSM_used = i-5;
      b=false;
    }
    if(millis() - start > 5000) {
      #ifdef DEBUG
        Serial.println(F("SIM timeout"));
        reportBug(69);
      #endif
      return false;
    }
  }
  return true;
}

void initGSM() {
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial2.println("AT");
}

//byte* getBatteryInfo() {
//  static byte re[3];
//  Serial2.println("AT+CBC");
//  if( awaitResponse() ) {
//    for(byte i = 0; i < buforGSM_used; i++) {
//      
//    }
//  }
//}
