#define RX2 16
#define TX2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  Serial.println("---Start---");

  String sendtoserver = "{\"data\":\"Twoja stara\"}";

  Serial2.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  Serial2.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  Serial2.println("AT+SAPBR=3,1,\"USER\",\"\"");
  Serial2.println("AT+SAPBR=3,1,\"PWD\",\"\"");
  Serial2.println("AT+SAPBR=1,1");
  Serial2.println("AT+SAPBR=2,1");


/********************GSM Communication Starts********************/
 
  if (Serial2.available())
  Serial.write(Serial2.read());
 
  Serial2.println("AT");
  delay(3000);
 
//  Serial2.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
//  delay(6000);
//  ShowSerialData();
// 
//  Serial2.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");//APN
//  delay(6000);
//  ShowSerialData();
// 
//  Serial2.println("AT+SAPBR=1,1");
//  delay(6000);
//  ShowSerialData();
// 
//  Serial2.println("AT+SAPBR=2,1");
//  delay(6000);
//  ShowSerialData();
 
 
  Serial2.println("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
 
  Serial2.println("AT+HTTPPARA=\"CID\",1");
  delay(6000);
  ShowSerialData();
 
//  StaticJsonBuffer<200> jsonBuffer;
//  JsonObject& object = jsonBuffer.createObject();
//  
//  object.set("deviceID",deviceID);
//  object.set("humidity",humidity);
//  object.set("temperature",temperature);
//  object.set("timedate",t);
//  
//  object.printTo(Serial);
//  Serial.println(" ");  
//  String sendtoserver;
//  object.prettyPrintTo(sendtoserver);
//  delay(4000);
 
  Serial2.println("AT+HTTPPARA=\"URL\",\"http://daniel.rozycki.student.put.poznan.pl/incomingData.php\""); //Server address
  delay(4000);
  ShowSerialData();
 
  Serial2.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(4000);
  ShowSerialData();
 
 
  Serial2.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
  Serial.println(sendtoserver);
  delay(6000);
  ShowSerialData();
 
  Serial2.println(sendtoserver);
  delay(6000);
  ShowSerialData;
 
  Serial2.println("AT+HTTPACTION=1");
  delay(6000);
  ShowSerialData();
 
  Serial2.println("AT+HTTPREAD");
  delay(6000);
  ShowSerialData();
 
  Serial2.println("AT+HTTPTERM");
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
