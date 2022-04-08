/*
 * Klasa ta ma zajmować się komunikacją z RTC oraz zarządzać trybem sleep (niskoprądowym)
 * Akurat ta klasa jest przeznaczona do komunikacji z rtc bez trybu alarmu (tylko odczyt daty), takim jak DS1307
 * Aby użyć klasy RTC z alarmem i budzeniem przez interrupty należy skorzystać z klasy TimerAlarm.h
 */

 // TODO podlaczenie rtc za pomoca Wire.h i i2c

#include "RTClib.h"
#include <LowPower.h>

class Timer {
  public:
    boolean DEBUG = false;
    RTC_DS1307 rtc;
    DateTime now;
    DateTime kiedy_poszedlem_spac;
    int licznik_godziny = 0;
    int podzialka_godzin;
    int reszta_podzialki;
    int ile_mam_spac;
    boolean spie = false;
    const int pin_wstrzas = 0;
    boolean idle_wakeup = true; // sam sie obudzilem, nie obudzil mnie interrupt
    boolean initiated_correctly = false;

    boolean init();
    boolean is_date_set();
    boolean set_time(int rok, int miesiac, int dzien, int godzina, int minuta, int sekunda);

    String print_DateTime(DateTime czas);

    void spij_godzine_synchronizowane();
    boolean spij_czas();
    void reakcja_na_wstrzas();
    static void reakcja_na_wstrzas_static();
};
