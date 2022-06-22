// esp8266 wersja

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "Pomiar.h"


// --- DANE DOSTEPOWE ITP
const char* ssid = "TcpToSerialServer";
//const char* password = "";

// --- WIFI i SIEC INIT
ESP8266WebServer server(80);

// --- ZMIENNE
//  -- przechowywanie pomiarow
#define max_wielkosc_pasieki 20
Pomiar zbior_pomiarow[max_wielkosc_pasieki]; //zbiera pomiary z danego okresu (np co pol godziny), zeby na koniec okresu je wszystkie razem wyslac
short i_zbior_pomiarow = 0; // w ktore miejce ma zapisac pomiar
//  -- czy ma debugowac info dla preprocesora
#define DEBUG true

void zajmijSieDanymi(String slaveID_str, String sekundy_str, String waga_str, String temperatura_str) {
  Pomiar current_pomiar(slaveID_str, waga_str, temperatura_str,sekundy_str);
  zbior_pomiarow[i_zbior_pomiarow] = current_pomiar;
  i_zbior_pomiarow++;

  #if DEBUG
    Serial.println( current_pomiar.toString() );
  #endif

  // TODO odpowiedz ze wszystko jest ok lub odpowiedz ze powinienes sobie ogarnac czas o +- tyle i tyle
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void handleCommDownloadAll() {
  String odpowiedz = "MSG:Pozdrawiam z esp, moj czas od wlaczenia: " + String(millis());
  char odpowiedz_downgrade[odpowiedz.length()];
  odpowiedz.toCharArray(odpowiedz_downgrade,odpowiedz.length());

  server.send(200, "text/plain" ,odpowiedz_downgrade);
  Serial.println("odpowiadam");
}

void handleOdbiorDanych() {
  // https://techtutorialsx.com/2016/10/22/esp8266-webserver-getting-query-parameters/

  String slaveID = server.arg("ID");
  String waga = server.arg("waga");
  String godzina = server.arg("godzina");
  // TODO protokół zwrotnej informacji jeśli godzina esp-slave odbiega za bardzo od prawdziwej, aby sobie dostosowało esp-slave
  String temperatura = server.arg("temp");

  zajmijSieDanymi(slaveID, godzina, waga, temperatura);
}

void handleRoot() {
  server.send(200, "text/html", "oops root responsem, method: "+server.method() );
}

void setup() {
  Serial.begin(38400);
  Serial.println("Zaczynam");
  
  delay(3000);

  // w przypadku resetu programowego,zeby wifi tez zresetowalo
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // if we want other than default 192.168.4.1
  IPAddress localIp(192,168,0,1);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);


  WiFi.softAPConfig(localIp,gateway,subnet);
  
  // WiFi.softAp(ssid,password,channel,hidden)
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP stworzone");
  Serial.print("Moje IP: ");
  Serial.print(myIP);
  Serial.println();

  server.on("/", handleRoot);
  server.on("/OdbiorDanych",handleOdbiorDanych);
  server.on("/CommDownloadAll",handleCommDownloadAll);
  server.onNotFound(handleNotFound);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
