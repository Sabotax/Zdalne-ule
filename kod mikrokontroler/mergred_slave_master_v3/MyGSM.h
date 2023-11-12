#define RX2 16
#define TX2 17

#define SerialMon Serial
#define SerialAT Serial2

// Your GPRS credentials, if any
const char apn[]      = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

bool connectingSuccess = false;
#define DUMP_AT_COMMANDS


// basic tools
String waitResponse(uint32_t t = 0){
  if(t==0) 
    delay(1000);
  else
    delay(t*1000);
    
  String re = "";
  while (SerialAT.available() != 0)
    re += (char) SerialAT.read();

  delay(1000);
  re.trim();
  #ifdef DUMP_AT_COMMANDS
    SerialMon.println("|"+re+"|");
  #endif

  
  return re;
}

void sendAT(String s) {
  #ifdef DUMP_AT_COMMANDS
    Serial.println("Wysylanie komendy AT: " + s);
  #endif
  
  SerialAT.println(s);
}
//todo signal strength

void resetSIM() {
  sendAT("AT+CFUN=0");
  waitResponse(3);
  sendAT("AT+CFUN=1,1");
  waitResponse(20);
}

bool isConnected() {
  sendAT("AT+CREG?");
  String re = waitResponse(1);
  if(re=="+CREG: 0,1\r\n\r\nOK")
    return true;
  else
    return false;
}

int getBattery() {
  sendAT("AT+CBC");
  String input = waitResponse(3);
  
  int startIndex = input.indexOf(',') + 1;
  int endIndex = input.indexOf(',', startIndex);
  String secondNumberString = input.substring(startIndex, endIndex);
  int secondNumber = secondNumberString.toInt();
  
  SerialMon.println("Extracted second number: " + secondNumberString); 
  return secondNumber;
}

int getSignal() {
  sendAT("AT+CSQ");
  String input = waitResponse(3);
  
  int startIndex = input.indexOf(':') + 2;
  int endIndex = input.indexOf(',', startIndex);
  
  String value = input.substring(startIndex, endIndex);
  int valueNumber = value.toInt();
  SerialMon.println("Extracted value number: " + value); 

  return valueNumber;
}

int getHttpStatus() {
  sendAT("AT+HTTPACTION=1");
  String input = waitResponse(10);
  
  int startIndex = input.indexOf(',') + 1;
  int endIndex = input.indexOf(',', startIndex);
  
  String value = input.substring(startIndex, endIndex);
  int valueNumber = value.toInt();
  SerialMon.println("Extracted value status number: " + value); 

  return valueNumber;
}

void handshakeGSM() {
  SerialAT.begin(38400, SERIAL_8N1, RX2, TX2);
  SerialAT.println("AT");
  waitResponse(2);
}

void initMyGSM() { // TODO init only if not connected
  handshakeGSM();

  if(!isConnected()) {
    sendAT("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    if(waitResponse(1)=="ERROR") {
      resetSIM();
    }
    sendAT("AT+SAPBR=3,1,\"APN\",\"internet\"");
    waitResponse(1);
    sendAT("AT+SAPBR=3,1,\"USER\",\"\"");
    waitResponse(1);
    sendAT("AT+SAPBR=3,1,\"PWD\",\"\"");
    waitResponse(1);
    sendAT("AT+SAPBR=1,1");
    waitResponse(1);
    sendAT("AT+SAPBR=2,1");
    waitResponse(1);

    //testowo tutaj
    //getBattery();
    //getSignal();
  }
  
}

String makePostGSM(String body, bool retry) {

  while(SerialAT.available()) SerialAT.read();
 
  sendAT("AT");
  waitResponse(3);

  byte counterRetry = 0;
  while(counterRetry < 3) {
    sendAT("AT+HTTPINIT");
    if(waitResponse(6)!="OK") {
      counterRetry++;
      resetSIM();
    }
    else break;
  }
  if(counterRetry == 3) return "FAIL";
 
  sendAT("AT+HTTPPARA=\"CID\",1");
  waitResponse(6);
 
  sendAT("AT+HTTPPARA=\"URL\",\""+serverAndRes+"\""); //Server address
  waitResponse(4);
 
  sendAT("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  waitResponse(4);
 
  sendAT("AT+HTTPDATA=" + String(body.length()) + ",100000");
  waitResponse(6);
 
  sendAT(body);
  waitResponse(6);
 
  int status = getHttpStatus();
 
  sendAT("AT+HTTPREAD");
  String response = waitResponse(10);
 
  sendAT("AT+HTTPTERM");
  waitResponse(6);

  if(status != 200 && retry) makePostGSM(body,false); // jak jest 601 to chbya trzeba restart sim w ogole od zasilania, pamietac o tranzystorze od tego jakims w przyszlosci?

  return response;
}
// todo sleep, wakeup
