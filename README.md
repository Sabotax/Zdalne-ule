# Zdalne-ule
Projekt przejściowy, Różycki Daniel. Harmonogram/plan, który zamierzam realizować (jako tako) -> https://tiny.pl/94gcx

Po wielu przejściach stworzono pojedynczą jednostkę mierzącą wagę, temperaturę wewnętrzną, zapisującą to na kartę działającą w ramach esp8266.  
W ramach tego semestru planuję jeszcze zrobić zaprogramować, by oprócz zapisywania komunikował się z drugim mikrokontrolerem (esp32) np poprzez SSH.  
Docelowo każda jednostka ma być na esp32, co umożliwi budzenie ze snu z pomocą wykrywacza wstrząsu, dodatkowo ma też wiele innych zalet ( w tym opcja low power bluetooth).  
Główna jednostka będzie posiadać RTC (DS3231), który ponadto może mierzyć temperaturę (zewnętrzną). DeepSleep w nie-głównych esp, jest na tyle dokładny, że wystarczy, by główna jednostka posiadała rtc, w którego ramach może synchronizować pozostałe.  
Główna jednostka będzie posiadała rtc, moduł gsm, panel oraz układ ładowania akumulatora (prawdopodobnie tp4056).  

Zdjęcie pojedynczej jednostki (dodatkowo z SD):
![offline_zbieranie_v3](/programy\offline_zbieranie_danych_v3/showcase_v3.jpg "Zestaw")
