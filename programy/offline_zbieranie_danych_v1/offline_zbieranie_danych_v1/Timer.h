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
    boolean DEBUG = true;
    RTC_DS1307 rtc;
    DateTime now;
    DateTime kiedy_poszedlem_spac;
    int licznik_godziny = 0;
    int podzialka_godzin;
    int reszta_podzialki;
    TimeSpan ile_mam_spac;
    const int pin_wstrzas = 2;
    boolean sen_przerwany = false; // sam sie obudzilem, nie obudzil mnie interrupt
    boolean initiated_correctly = false;

    boolean init();
    boolean set_time(int rok, int miesiac, int dzien, int godzina, int minuta, int sekunda);

    String print_DateTime(DateTime czas);

    void spij_start(int godziny, int minuty, int sekundy);
    void spij_czas(int czas);
    void reakcja_na_wstrzas();
    static void reakcja_na_wstrzas_static();
};
