/*
    This sketch sends a string to a TCP server, and prints a one-line response.
    You must run a TCP server in your local network.
    For example, on Linux you can use this command: nc -v -l 3000
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "CGA2121_Tu7GnYu"
#define STAPSK  "mtyRdz7KZcEc9k2Ezw"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.0.19";
const uint16_t port = 23;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  // This will send the request to the server
  

  //read back one line from server
  Serial.println("receiving from remote server");
  String line = client.readStringUntil('\n');
  Serial.println(line);
  client.println("ESP");
  delay(1000);
  client.println("haslo");
  //Serial.println("closing connection");
  //client.stop();

  while(true) {
    Serial.println("wait 5 sec...");
    client.println("hello from ESP8266");
    delay(5000);
  }

}
