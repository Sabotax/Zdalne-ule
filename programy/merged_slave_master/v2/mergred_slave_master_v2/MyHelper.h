// tutaj zrobić reportowanie buga dobre (z opcją ledową) i helpery jsonowe
const String espId = "esp01";
const String authToken = "Watykanczyk2137";

String dataToJson(const float& waga, const String& myTimestamp) {

  String re = "{";
  re += "\"auth\":"; re+= "\""+espId+"\",";
  re += "\"id\":"; re+= "\""+authToken+"\",";
  re += "\"waga\":"; re+= "\""+String(waga)+"\",";
  re += "\"time\":"; re+= "\""+myTimestamp+"\"";
  re += "}";

  return re;
}

String dataToCsvRow(const float& waga, const String& myTimestamp) {

  String re = "";
  re += espId+";";
  re+= String(waga)+";";
  re+= myTimestamp+"\r\n";

  return re;
}

//SLEEP
#define TIME_TO_SLEEP  120
#define uS_TO_S_FACTOR 1000000

// funkcje
void handle_sleep() {
  Serial.println("Czas spania: " + String(TIME_TO_SLEEP * uS_TO_S_FACTOR) + " sekund" );
  Serial.flush();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
