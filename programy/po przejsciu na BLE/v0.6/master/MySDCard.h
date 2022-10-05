/*
 * Założenia:
 * -zapisuje dane na kartę na plik tekstowy (każdego dnia jest inny plik tekstowy), a poszczególne krotki dodaje w linii
 * -telefon będzie mógł zdobyć te dane i przechować (nie wiadomo jeszcze czy używając BLE czy włączanego na chwilę wifi)
 * -następnie telefon wyśle dane i przechowa odpowiedź serwera (który wkładając sprawdza, żeby się nie powtarzały krotki itp) mówiącą od którego dnia i którego esp ma dane
 * -następnie przy kolejnym łączeniu z tym konkretnym esp, esp dostanie dane do którego dnia może usunąć dane (bo znajdują się już na serwerze)
 * 
 */
