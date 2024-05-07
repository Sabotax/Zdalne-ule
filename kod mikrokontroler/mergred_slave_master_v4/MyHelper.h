// tutaj zrobić reportowanie buga dobre (z opcją ledową) i helpery jsonowe
const int espId = 0;

bool bleWakeUp = false;
long bleWakeUpMoment=0;
#define Threshold 80

#define pinWakeUpButton GPIO_NUM_32
#define pinWakeUpLed 33

uint8_t ledStance = 2;

String dataToSend = "";
bool sendingData = false;

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
    case ESP_SLEEP_WAKEUP_EXT0 : 
    Serial.println("Wakeup caused by external signal using RTC_IO");
    #ifndef wakeUpTouch
      bleWakeUp=true;
      bleWakeUpMoment=millis();
    #endif
    break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
      Serial.println("Wakeup caused by touchpad"); 
      #ifdef wakeUpTouch
        bleWakeUp=true;
        bleWakeUpMoment=millis();
      #endif
      break;
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
void enableGpioWakeUp() {
  esp_sleep_enable_ext0_wakeup(pinWakeUpButton,1);
}
void convertIntToThree8uint(uint8_t* arrayToBeFilled, int convertedNumber) {
  uint32_t workingNumber;
  if(convertedNumber < 0) workingNumber = 0;
  else workingNumber = (uint32_t) convertedNumber;

  arrayToBeFilled[0] = (workingNumber & 0x000000ff);
  arrayToBeFilled[1] = (workingNumber & 0x0000ff00) >> 8;
  arrayToBeFilled[2] = (workingNumber & 0x00ff0000) >> 16;
}
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
uint32_t ileSekundPrzerwy = 1800;
bool czasZapisu = false;

void ARDUINO_ISR_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
  czasZapisu = true;
}

void initTimer() {
  timerSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(0, 80000000, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, ileSekundPrzerwy, true);
  timerAlarmEnable(timer);
}
