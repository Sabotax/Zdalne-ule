#include "RTClib.h"
#include <Wire.h>
RTC_DS3231 rtc;

void initRTC() {
  if (! rtc.begin()) {
    #ifdef DEBUG
    Serial.println("Couldn't find RTC");
    Serial.flush();
    #endif
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    #ifdef DEBUG
    Serial.println("RTC lost power, let's set the time!");
    #endif
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

String getTimestamp() {
  DateTime now = rtc.now();
  String re = "";
  re += String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " ";
  re += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  return re;
}

String getDay() {
  DateTime now = rtc.now();
  String re = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day());
  return re;
}
