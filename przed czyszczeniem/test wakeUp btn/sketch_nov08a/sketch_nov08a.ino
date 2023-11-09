#define pinWakeUpButton GPIO_NUM_32
#define pinWakeUpLed 33

bool bleWakeUp = false;
long bleWakeUpMoment=0;

//SLEEP
#define TIME_TO_SLEEP  60
#define uS_TO_S_FACTOR 1000000

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
    case ESP_SLEEP_WAKEUP_EXT1 : 
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
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

void enableGpioWakeUp() {
  esp_sleep_enable_ext0_wakeup(pinWakeUpButton,1);
}

void handle_sleep() {
  Serial.println("Czas spania: " + String(TIME_TO_SLEEP) + " sekund" );
  Serial.flush();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void setup() {
  setCpuFrequencyMhz(80); //dla hx711
  Serial.begin(115200);

  delay(5000);
  Serial.println("--START--");

  digitalWrite(pinWakeUpLed,LOW);
  pinMode(pinWakeUpLed,OUTPUT);
  // put your setup code here, to run once:
  enableGpioWakeUp();
  didIwakeUpForBle();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bleWakeUp) {
    digitalWrite(pinWakeUpLed,HIGH);
    if( millis() > bleWakeUpMoment + (1000*60*5)) {
      // time's up, going to sleep
      Serial.println("time's up, going to sleep " + String(millis() ) + " " + String(bleWakeUpMoment));
      digitalWrite(pinWakeUpLed,LOW);
      handle_sleep();
    }
    else {
      Serial.println("I am awake");
      delay(5000);
    }
  }
  else {
    handle_sleep();
  }
}
