/*
 * Założenia:
 * -zapisuje dane na kartę na plik tekstowy (każdego dnia jest inny plik tekstowy), a poszczególne krotki dodaje w linii
 * -telefon będzie mógł zdobyć te dane i przechować (nie wiadomo jeszcze czy używając BLE czy włączanego na chwilę wifi)
 * -następnie telefon wyśle dane i przechowa odpowiedź serwera (który wkładając sprawdza, żeby się nie powtarzały krotki itp) mówiącą od którego dnia i którego esp ma dane
 * -następnie przy kolejnym łączeniu z tym konkretnym esp, esp dostanie dane do którego dnia może usunąć dane (bo znajdują się już na serwerze)
 * 
 * odnośnie przesyłania danych na telefon:
 * można skorzystać z BLE, (max 512 bajtów na usługę, czyli 512 znaków) ale trzeba w takim razie zrobić coś w stylu: przesyła około 500 znaków, kończy przesyłanie ich gdzieś 
 * (może w drugiej usłudze) informuje że dalej trzeba i że jeszce nie skończyło i dalej leci przesyłanie i tak w kółko, dzięki temu "protokołowi przesyłania większych ilości danych
 * over BLE" nie będzie problemów z włączaniem wifi i hotspot itp, więc tak będzie chyba lepiej, prądowo też
 * 
 * fun fact, dzięki wystawieniu informacji na początku jak duży jest plik/jego zawartość będzie można zrobić progress bar zwiększający się co iteracje tego protokołu
 */
