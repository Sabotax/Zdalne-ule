#define RX2 16
#define TX2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  Serial.println("---Start---");
  Serial.println("Resetting SIM");
  resetSIM();

  String sendtoserver = "{\"auth\":\"Watykanczyk2137\",\"id\":0,\"waga\":\"59.00\",\"time\":1692811966,\"batt\":\"20\"} ";

  sendAT("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  sendAT("AT+SAPBR=3,1,\"APN\",\"internet\"");
  sendAT("AT+SAPBR=3,1,\"USER\",\"\"");
  sendAT("AT+SAPBR=3,1,\"PWD\",\"\"");
  sendAT("AT+SAPBR=1,1");
  sendAT("AT+SAPBR=2,1");


/********************GSM Communication Starts********************/
 
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
  Serial.println(sendtoserver);
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

  // WNIOSKI -> sukces, trzeba apn ustawić żeby działało
  /********************GSM Communication Stops********************/
}

void loop() {
  // put your main code here, to run repeatedly:

 
}
 
 
void ShowSerialData()
{
  while (Serial2.available() != 0)
    Serial.write(Serial2.read());
  delay(1000);
 
}

void resetSIM() {
  sendAT("AT+CFUN=0");
  delay(2000);
  sendAT("AT+CFUN=1,1");
  delay(15000);
}

void sendAT(String s) {
  Serial.println("Komenda: " + s);
  Serial2.println(s);
}


/*
 * tak wyglada w przypadku faila
 * Resetting SIM
20:25:40.036 -> 
20:25:49.045 -> +SAPBR 1: DEACT
20:25:49.045 -> 
20:25:49.045 -> +CPIN: NOT READY
20:25:49.045 -> 
20:25:49.045 -> OK
20:25:49.045 -> 
20:25:49.045 -> ERROR
20:25:49.045 -> 
20:25:49.045 -> ERROR
20:25:56.017 -> 
20:25:56.017 -> ERROR
20:26:01.041 -> 
20:26:01.041 -> ERROR

trzeba znowu zrobic restart i moze zwiekszyc czekanie do 20/30 sekund i po tym ponownie sie poprawnie laczy

Todo trzeba to w kodzie obsluzyc
 */
