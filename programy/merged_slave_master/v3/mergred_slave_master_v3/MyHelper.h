// tutaj zrobić reportowanie buga dobre (z opcją ledową) i helpery jsonowe
const int espId = 0;


bool bleWakeUp = false;
long bleWakeUpMoment=0;
#define Threshold 80

String dataToJson(const float& waga, const String& myTimestamp, const String& battery) {

  String re = "{";
  re += "\"auth\":"; re+= "\""+authToken+"\",";
  re += "\"id\":"; re+= String(espId)+",";
  re += "\"waga\":"; re+= String(waga)+",";
  re += "\"time\":"; re+= String(myTimestamp)+",";
  re += "\"batt\":"; re+= "\""+battery+"\"";
  re += "}";

  return re;
}

String dataToCsvRow(const float& waga, const String& myTimestamp) {

  String re = "";
  re+= String(waga)+",";
  re+= myTimestamp+";";

  return re;
}

//SLEEP
#define TIME_TO_SLEEP  1800
#define uS_TO_S_FACTOR 1000000

// funkcje
void handle_sleep() {
  Serial.println("Czas spania: " + String(TIME_TO_SLEEP) + " sekund" );
  Serial.flush();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void didIwakeUpForBle() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); bleWakeUp=true;bleWakeUpMoment=millis();break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }

}

void callback(){
  //placeholder callback function
}

void enableTouchWakeUp() {
  //Setup interrupt on Touch Pad 1 (GPIO 0)
  touchAttachInterrupt(T9, callback, Threshold);
  touchAttachInterrupt(T8, callback, Threshold);

  //Configure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();
}
