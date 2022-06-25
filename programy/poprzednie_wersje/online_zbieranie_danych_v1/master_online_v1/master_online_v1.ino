#include <ESP8266WiFi.h>
#define NUM_SLAVES 1
String nom = "Master";
const char* ssid = "CGA2121_Tu7GnYu";
const char* password = "mtyRdz7KZcEc9k2Ezw";
bool sendCmd = false;
String slaveCmd = "0";
String slaveState = "0";
WiFiServer server(80);
WiFiClient browser;
IPAddress ip(192, 168, 0, 44);
IPAddress gateway(192, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  server.begin();
  Serial.print(nom);
  Serial.print(F(" connected to Wifi! IP address : http://")); 
  Serial.println(WiFi.localIP()); // Print the IP address
}

void loop() {
  clientRequest();
}
void clientRequest( ) { /* function clientRequest */
 ////Check if client connected
 WiFiClient client = server.available();
 client.setTimeout(50);
 if (client) {
   if (client.connected()) {
     //Print client IP address
     Serial.print(" ->");Serial.println(client.remoteIP());
     String request = client.readStringUntil('\r'); //receives the message from the client
    
     if (request.indexOf("Slave0") == 0) {
       //Handle slave request
       Serial.print("From "); 
       Serial.println(request);
       int index = request.indexOf(":");
       String slaveid = request.substring(0, index);
       slaveState = request.substring(request.indexOf("x") + 1, request.length());
       Serial.print("state received: "); 
       Serial.println(slaveState);
//       client.print(nom);
//       if (sendCmd) {
//         sendCmd = false;
//         client.println(": Ok " + slaveid + "! Set state to x" + String(slaveCmd) + "\r");
//       } else {
//         client.println(": Hi " + slaveid + "!\r"); // sends the answer to the client
//       }
       client.stop();                // terminates the connection with the client
     } 
//     else {
//       Serial.print("From Browser : "); Serial.println(request);
//       client.flush();
//       handleRequest(request);
//       webpage(client);
//     }
   }
 }
}
