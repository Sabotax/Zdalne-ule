/*
 * Kod ma używać ESP.deepsleep(0) aby się usypiac w nieskonczonosc. 
 * Normalnie na RST jest HIGH, jak się przełączy w LOW, to esp się resetuje.
 * Tak działa ustawienie ESP.deepsleep(X), gdzie podłącza się D0 do reset, a ESP po X nanosekund przełącza D0 w stan niski co resetuje esp.
 * W przypadku użycia do tego zewnętrznego rtc z alarmami, jeśli chcemy prawdziwe deepsleep (działające jak powyżej) ale też dokładność rtc DS3231 to musimy podłączyć pin alarmowy
 * SQW do reseta.
 * Pin SQW na rtc działa tak, że jest na HIGH cały czas, a gdy alarm wystąpi (alarm 1 lub alarm 2), to na SQW wystawia LOW.
 * Po wystawieniu LOW będzie tak długo, aż programowo się nie zdejmie flag acknowledge itp.
 * Czyli po wystąpieniu co prawda wystawi na RST low, ale esp będzie wyłączone tak długo jak to low będzie, czyli nigdy się nie włączy z powrotem, bo musi być LOW a później HIGH
 * aby się włączyć.
 * Proponowane rozwiązanie nr 1:
 * Użyć układu z kondensatorem i rezystorem podciągającym aby ze zmiany SQW HIGH->LOW zrobić impuls LOW, a nie stałą zmianę.
 * W takim wypadku wystarczy raz zaprogramować RTC by ustawić alarm np co godzinę, a w setup esp dać, że na starcie acknowleduje alarmy i leci (+ pobiera datę, powód alarmu
 * (jeśli możliwe, bo ma teoretycznie byc ten czujnik wstrzasu, ale to może w przyszłości, nie jest to główna funcjonalność, a wiele może popsuć próbowanie się dostosowania teraz)
 * 
 */

 /* KONIECZNY HARDWARE
  * jako że zamiast czegoś takiego jak interrupt pin uzywamy SQW do resetowania esp poprzez RST, to trzeba zrobic kilka rzeczy
  * od sqw dac 100nF lub 470nF kondensator szeregowo, nastepnie pull-up rezystor 3.3V, 10k ohm i dopiero wtedy RST
  * 
  */
#define countof(a) (sizeof(a) / sizeof(a[0]))
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring, 
      countof(datestring),
      PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
      dt.Month(),
      dt.Day(),
      dt.Year(),
      dt.Hour(),
      dt.Minute(),
      dt.Second() );
    Serial.print(datestring);
}
void setup() {
  Serial.begin(74880);

  // -------RTC INIT-------
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          Serial.println("RTC lost confidence in the DateTime!");
          Rtc.SetDateTime(compiled);
      }
  }

  if (!Rtc.GetIsRunning())
  {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
      Rtc.Enable32kHzPin(false);
      Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne); 
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
      now = compiled;
  }
  //wszystkie flagi: https://github.com/Makuna/Rtc/blob/master/src/RtcDS3231.h

  // -------RTC SET ALARM------
  // jesli nie jest ustawiony alarm, to go ustaw
  //if ( Rtc.GetAlarmOne() == DS3231AlarmOne(0, 0, 0, 0, DS3231AlarmOneControl_HoursMinutesSecondsDayOfMonthMatch) ) {
  if ( 1 ) {
    // debugowo:  DS3231AlarmOneControl_SecondsMatch 
    // normalnie: DS3231AlarmOneControl_MinutesSecondsMatch 
    // pozniej tez bedzie stricte minuta i sekunda, zeby wszystkie ule sie synchronizowaly
    DS3231AlarmOne alarm1( now.Day(), now.Hour(),now.Minute(), now.Second(), DS3231AlarmOneControl_OncePerSecond );
    Rtc.SetAlarmOne( alarm1 );
    Serial.print("Alarm nieustawiony - ustawiam alarm1 na ");
    printDateTime(now);
    Serial.println("Seconds Match");
  }

  // ------ESP potwierdz alarm ------
  Rtc.LatchAlarmsTriggeredFlags();
  Serial.println("Uwalniam flagi");

  // ------ESP zbierz dane------
  int liczba = millis();

  // ------ESP zapisz dane ------
  Serial.println("Zbieram dane");
  printDateTime(now);
  Serial.println(" pomiar: " + String(liczba));

  // ----- ESP idz spac-------
  Serial.flush();
  ESP.deepSleep(0);
  
}

void loop() {
}
