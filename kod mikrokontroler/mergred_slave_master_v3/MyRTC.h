//RTC
#include "RTClib.h"
#include <Wire.h>
RTC_DS3231 rtc;
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
String getEpoch() {
  DateTime now = rtc.now();
  return String(now.unixtime());
}
String getTimestamp() {
  DateTime now = rtc.now();

  String myDay = getDay();
  String myHour = String(now.hour());
  if(myHour.length()==1) myHour = "0"+myHour;
  String myMinute = String(now.minute());
  if(myMinute.length()==1) myMinute = "0"+myMinute;
  String mySec = String(now.second());
  if(mySec.length()==1) mySec = "0"+mySec;
  String myHourAll = myHour + ":" + myMinute + ":" + mySec;

  String re = myDay + "T" + myHourAll;

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
    Serial.println("Setting time to " + String(F(__DATE__)) + "|" + String(F(__TIME__)));
    #endif
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void setTimeRTC(uint32_t epoch_second) {
  rtc.adjust(DateTime(epoch_second));
}
