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
    if (! rtc.isrunning()) {
    if (DEBUG) Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    now = DateTime(F(__DATE__), F(__TIME__));
    if (DEBUG) Serial.println("Ustawiam czas na " + print_DateTime(now));
    rtc.adjust(now);
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
    if (DEBUG) Serial.println("RTC initialized correctly");
    initiated_correctly = true;
    return true;
  }
}

boolean Timer::set_time(int rok, int miesiac, int dzien, int godzina, int minuta, int sekunda) {
  // (rok, miesiac, dzien, godzina, minuta, sekunda)
  rtc.adjust( DateTime(rok, miesiac, dzien, godzina, minuta, sekunda) );
  if (DEBUG) Serial.println("Ustawiono datę RTC");
  return true;
}

void Timer::spij_start(int godziny, int minuty, int sekundy) {
  now = rtc.now();
  if (DEBUG) Serial.println("Jest " + print_DateTime(now));
  DateTime czas_ktory_powinien_byc (kiedy_poszedlem_spac + TimeSpan(0,godziny,minuty,sekundy));
  if (DEBUG) Serial.println("Powinno byc " + print_DateTime(czas_ktory_powinien_byc));
  TimeSpan roznica = now - czas_ktory_powinien_byc;
    if (!sen_przerwany) {
    // sen nie został przerwany
    kiedy_poszedlem_spac = now;
  }
  else {
    sen_przerwany = false;
  }

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
  ile_mam_spac = TimeSpan(0,godziny,minuty,sekundy) - roznica ;
  //if (DEBUG) Serial.println("Ide spac do " + ile_mam_spac);
  if (DEBUG) Serial.println("Ide spac na " + String(ile_mam_spac.totalseconds()) );
  if (DEBUG) delay(20);
  spij_czas(ile_mam_spac.totalseconds());
  return;
}

void Timer::spij_czas(int czas) {
  int n = czas / 8;
  int reszta = czas % 8;

  for (int i = 0 ; i < n ; i++) {
    LowPower.powerDown(SLEEP_8S,ADC_OFF, BOD_OFF);
  }
  if (reszta >= 4) {
    reszta -= 4;
    LowPower.powerDown(SLEEP_4S,ADC_OFF, BOD_OFF);
  } 
  else if (reszta >= 2) {
    reszta -= 2;
    LowPower.powerDown(SLEEP_2S,ADC_OFF, BOD_OFF);
  }
  else if (reszta >= 1) {
    reszta -= 1;
    LowPower.powerDown(SLEEP_1S,ADC_OFF, BOD_OFF);
  }
  
  return;
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
  sen_przerwany = true;
  if (DEBUG) Serial.println("Alarm, ktos mnie szturchnal");
  Save1.save("alarmy.txt",Timer::print_DateTime(rtc.now()) + "- interrupt z czujnika wstrzasow");
  // gps itp powiadomienie
  Timer::spij_start(0,0,10);
}

void Timer::reakcja_na_wstrzas_static() {
  Timer1.reakcja_na_wstrzas();
}
