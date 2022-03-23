 /***************  Telnet *********************/
 #include <ESP8266WiFi.h>
WiFiServer TelnetServer(23); // Telnet Server Declaration port 23
WiFiClient SerialOTA;     // Telnet Client Declaration 
bool haveClient = false; client detection flag


/*************** Setup *********************/
void setup()
{
//after OTA setup

timer.setInterval(1000L, UpTime);

/*************** Telnet  *********************/
  TelnetServer.begin();
  TelnetServer.setNoDelay(true);
//end setup

void loop() {
  timer.run();
  Blynk.run();
  ArduinoOTA.handle();  // For OTA

// Handle new/disconnecting clients.
  if (!haveClient) {
    // Check for new client connections.
    SerialOTA = TelnetServer.available();
    if (SerialOTA) {
      haveClient = true;
      Blynk.setProperty(Vx, "color", BLYNK_GREEN); //TELNET LED ON
    }
  } else if (!SerialOTA.connected()) {
    // The current client has been disconnected.
    SerialOTA.stop();
    SerialOTA = WiFiClient();
    haveClient = false;
    Blynk.setProperty(Vx, "color", DARK_RED);  //TELNET LED OFF
  }
}


void UpTime() { // to print something 

SerialOTA.print("time: ");
  SerialOTA.println(millis());
}
