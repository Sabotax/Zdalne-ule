/*
 * Klasa ta ma zajmować się komunikacją z RTC oraz zarządzać trybem sleep (niskoprądowym)
 * Akurat ta klasa jest przeznaczona do komunikacji z rtc bez trybu alarmu (tylko odczyt daty), takim jak DS1307
 * Aby użyć klasy RTC z alarmem i budzeniem przez interrupty należy skorzystać z klasy TimerAlarm.h
 */
// TODO LowPower.h
#include "RTClib.h"

class Timer {
  public:
    boolean DEBUG = false;
    RTC_DS1307 rtc;
};
