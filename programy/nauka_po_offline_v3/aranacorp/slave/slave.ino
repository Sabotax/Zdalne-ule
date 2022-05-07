// przykladowy - com10
//Libraries
#include <ESP8266WiFi.h>//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
//Constants
#define LED D4
#define UPDATE_TIME 500
//Parameters
String nom = "Slave0";
const char* ssid = "CGA2121_Tu7GnYu";
const char* password = "mtyRdz7KZcEc9k2Ezw";
//Variables
String command;
unsigned long previousRequest = 0;
//Objects
WiFiClient master;
IPAddress server(192, 168, 0, 44);
void setup() {
 //Init Serial USB
 Serial.begin(115200);
 Serial.println(F("Initialize System"));
 //Init ESP8266 Wifi
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(F("."));
 }
 Serial.print(nom);
 Serial.print(F(" connected to Wifi! IP address : ")); Serial.println(WiFi.localIP()); // Print the IP address
 pinMode(LED, OUTPUT);
}
void loop() {
 requestMaster();
}
void requestMaster( ) { /* function requestMaster */
 ////Request to master
 if ((millis() - previousRequest) > UPDATE_TIME) { // client connect to server every 500ms
   previousRequest = millis();
   if (master.connect(server, 80)) { // Connection to the server
     master.println(nom + ": Hello! my current state is x" + String(!digitalRead(LED)) + "\r");
     //answer
     String answer = master.readStringUntil('\r');   // receives the answer from the sever
     master.flush();
     Serial.println("from " + answer);
     if (answer.indexOf("x") >= 0) {
       command = answer.substring(answer.indexOf("x") + 1, answer.length());
       Serial.print("command received: "); Serial.println(command);
       if (command == "1") {
         Serial.println("LED ON");
         digitalWrite(LED, LOW);
       } else {
         Serial.println("LED OFF");
         digitalWrite(LED, HIGH);
       }
     }
   }
 }
}
