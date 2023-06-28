//RTC
#include "RTClib.h"
#include <Wire.h>
RTC_DS3231 rtc;

String getTimestamp() {
  DateTime now = rtc.now();
  String re = "";
  re += String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + "T";
  re += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  return re;
}

String getDay() {
  DateTime now = rtc.now();
  String my_year = String(now.year());
  String my_month = String(now.month());
  String my_day = String(now.day());
  if(my_month.length()==1) my_month = "0"+my_month;
  if(my_day.length()==1) my_day = "0"+my_day;
  String re = my_year + "-" + my_month + "-" + my_day;
  return re;
}

void initMyRTC() {
  //RTC
  if (! rtc.begin()) {
    #ifdef DEBUG
      Serial.println("Couldnt find RTC");
    #endif
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    #ifdef DEBUG
    Serial.println("RTC lost power, let's set the time!");
    #endif
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}
