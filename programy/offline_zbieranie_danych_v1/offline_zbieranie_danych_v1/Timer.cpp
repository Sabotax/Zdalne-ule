#include "Timer.h"
#include "Save.h"
extern Save Save1;
extern Timer Timer1;

boolean Timer::init() {
  pinMode(pin_wstrzas,INPUT);
  attachInterrupt(pin_wstrzas, reakcja_na_wstrzas_static, LOW);
  if ( !rtc.begin() ) {
    if (DEBUG) Serial.println("Couldn't find RTC");
    return false;
  }
  else {
    return true;
  }
}

boolean Timer::is_date_set() {
  if (! rtc.isrunning()) {
    if (DEBUG) Serial.println("RTC is NOT running");
    return false;
  }
  else {
    return true;
  }
}

boolean Timer::set_time(int rok, int miesiac, int dzien, int godzina, int minuta, int sekunda) {
  // (rok, miesiac, dzien, godzina, minuta, sekunda)
  rtc.adjust( DateTime(rok, miesiac, dzien, godzina, minuta, sekunda) );
  if (DEBUG) Serial.println("Ustawiono datę RTC");
  return true;
}

void Timer::spij_godzine_synchronizowane() {
  now = rtc.now();
  DateTime czas_ktory_powinien_byc (kiedy_poszedlem_spac + TimeSpan(0,1,0,0));
  TimeSpan roznica = now - czas_ktory_powinien_byc;

  //np
  // teraz to 13
  // powinno byc 14
  // roznica to -1 wiec pojdzie spac na godzine - (-minute) wiec dorowna do tego co powinno byc)

  //przyklad 2
  // teraz to 01:15
  // powinno byc 01:14
  // roznica to minuta
  // pojdzie spac na godzine - minuta wiec o 02:14 dorowna

  //milliseconds
  //ile_mam_spac = (60*60*1000) - (roznica.totalseconds()*1000); godzina - docelowo to ma byc
  //debug:
  ile_mam_spac = ( 60 -roznica.totalseconds()  )*1000;
  while ( spij_czas() );
  return;
}

boolean Timer::spij_czas() {
  if(spie) {
    if(licznik_godziny < podzialka_godzin) {
      licznik_godziny++;
      LowPower.powerDown(SLEEP_8S,ADC_OFF, BOD_OFF);
    } else {
      if(reszta_podzialki >= 4) {
        reszta_podzialki -= 4;
        LowPower.powerDown(SLEEP_4S,ADC_OFF, BOD_OFF);
      } else if(reszta_podzialki >= 2) {
        reszta_podzialki -= 2;
        LowPower.powerDown(SLEEP_2S,ADC_OFF, BOD_OFF);
      } else if(reszta_podzialki >= 1) {
        reszta_podzialki -= 1;
        LowPower.powerDown(SLEEP_1S,ADC_OFF, BOD_OFF);
      } else {
        spie = false;
        idle_wakeup = true;
      }
    }
  }
  else {
    podzialka_godzin = ile_mam_spac / 8; 
    reszta_podzialki = ile_mam_spac % 8;
    spie = true;
    if (idle_wakeup) kiedy_poszedlem_spac = rtc.now();
    licznik_godziny = 1;
    LowPower.powerDown(SLEEP_8S,ADC_OFF, BOD_OFF);
  }
  return spie;
}

String Timer::print_DateTime(DateTime ob) {
  String re = "";
  for( int i = 0; i < 6 ; i++) {
    switch(i) {
      case 0:
        re+=ob.year();
        break;
      case 1:
        re+=ob.month();
        break;
      case 2:
        re+=ob.day();
        break;
      case 3:
        re+=ob.hour();
        break;
      case 4:
        re+=ob.minute();
        break;
      case 5:
        re+=ob.second();
        break;
    }
    re+=":";
  }
  return re;
}

void Timer::reakcja_na_wstrzas() {
  spie = false;
  idle_wakeup = false;
  if (DEBUG) Serial.println("Alarm, ktos mnie szturchnal");
  Save1.save("alarmy.txt",Timer::print_DateTime(rtc.now()) + "- interrupt z czujnika wstrzasow");
  // gps itp powiadomienie
  Timer::spij_godzine_synchronizowane();
}

void Timer::reakcja_na_wstrzas_static() {
  Timer1.reakcja_na_wstrzas();
}
