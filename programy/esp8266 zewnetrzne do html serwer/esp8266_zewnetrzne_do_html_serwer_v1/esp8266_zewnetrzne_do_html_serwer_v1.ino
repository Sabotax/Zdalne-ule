#include <SoftwareSerial.h>

char receivedChars[numChars];

boolean newData = false;

SoftwareSerial wifiSerial(2, 3);      // RX, TX for ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout

void setup() {
  pinMode(13,OUTPUT);  //set build in led as output
  Serial.begin(115200);
  wifiSerial.begin(115200);

  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); // configure as access point
  sendToWifi("AT+CIFSR",responseTime,DEBUG); // get ip address
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); // turn on server on port 80
  sendToWifi("AT+CIPSTO=180",responseTime,DEBUG); // sever connection time
  //Serial.println((char)wifiSerial.read());
 
  sendToUno("Wifi connection is running!",responseTime,DEBUG);

//to be continued
}

void loop() {
  // put your main code here, to run repeatedly:

}
