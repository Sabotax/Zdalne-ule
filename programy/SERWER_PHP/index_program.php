<?php
    require_once("connect.php");
    require_once("library.php");

    $polaczenie = @new mysqli($host , $db_user, $db_password, $db_name);

    $tab = getSelectResponse(false,"SELECT * FROM `pomiary`",$polaczenie);

   // print_r($tab);

    echo "<table>";
    foreach($tab as $row) {
        echo "<tr>";
        foreach($row as $cell) {
            echo "<td>",$cell,"</td>";
        }
        echo "</tr>";
    }
    echo "</table>";
    /*
    TODO

    tabela esp-master
    tabela zmian esp-master (głównie lokacji)
    tabela rekordów(krotek) (pojedyncze pomiary)
    tabela gdzie dopisuje sie informacja gdy ul zmienił swojego esp-mastera

    dzięki temu i będzie widać gdy przeniesiono pasiekę (pionowa kreska na chart), ale też gdy pojedynczy ul
    wciąż można wyświetlać dane dla danego ula z każdego czasu, dnia, tygodnia, od czasu zmiany jego esp-mastera, ale także od czasu zmiany
    jego esp-mastera (trzeba wymyślić jak oba się zmienią w jakimś okresie czasu, żeby to smart się robiło (albo i nie robić))

    później też można tabelę tare zrobić gdzie każdy ul dostaje tare w danym okresie, i to wpływa na wykresy (wtedy wykres różnicowy byłby od ostatniego tare, ale nie tylko,
    po prostu w dacie gdzie ustawiono tare to byłaby wyjściowa waga, a reszta od niej by się odejmowała)

    podsumowując
    tabele:
    esp-master (id-idesp-lokacja-nazwa)
    esp-master-zmiany (id-id_esp_master-data-nowa_lokacja-nowa_nazwa-id_poprzedniego_najnowszego_rekordu_gdzie_bylo_zmieniane[zeby mozna cala sciezke zmian przesledzic po nitce])
    esp-slave (id_esp_slave-<nazwa aka_nr_ula>) [tu już zakładamy że jak zmieni ul nad esp-slave to trudno, monitorowanie zmian tego jest nieopłacalne, zmiana nazwy starczy]
    pomiary (id-id_esp_master-id_esp_slave-waga-temperatura-data)
    esp-slave-zmiany (id-id_esp_slave-data-nowe_esp_master-nowa_nazwa(opcjonalnie)-id_poprzedniego_najnowszego_rekordu-czy_tarowano_np)

    czynności:
    1. dodajemy nowe esp-master -> nowy wiersz w tabeli esp-master
    2. zmieniamy lokacje lub/i nazwe esp-master -> update do tabeli esp-master i insert do tabeli esp-master-zmiany
    3. pomiary się wykonują -> lecą pomiary do tabeli pomiary
        JEDNAK NIE, BO PUNKT 6 a) wykryje dla jakiegoś ula, że wcześniej miał inne esp-master -> dodatkowo insert do tabeli esp-slave-zmiany
    4. zmiana nazwy esp-slave -> insert do esp-slave-zmiany
    5. dodanie nowego esp-slave do siec i -> insert into esp-slave (domyslna nazwa bazowana na esp-slave-id) oraz insert into esp-slave-zmiany początkowego inicjującego rekordu
    6. esp-slave zostaje przeniesiony i się podłącza sam do esp-master (ale inne niż wcześniej[to dać do tego protokołu dołączania gdzie dostosowuje czas]) -> insert into esp-slave-zmiany
    7. esp-slave jest tarowany -> insert into esp-slave-zmiany

    pamietać że id-esp-slave to unikalne id tabeli esp-slave, ale będzie też unikalny identyfikator (trochę jak MAC) dla każdego esp-slave
    a nie, jednak nie bo skoro jest unikalny to może być jednocześnie kluczem tabeli, tak będzie lepiej, może tak samo dla esp-master jego id i id tabeli to to samo zrobić

    plan do zrobienia:
    1. esp-slave do esp-master wysyła standardowo swoje id i dane
    2. esp-master odbiera dane i zwiera je w tablicę i wysyła pomiary wraz z datą i swoim id na serwer
        wszystkie operacje i fikołki będą na serwerze robione, zrobić to tak żeby esp niezmiennie wysyłało id swoje,datę, i wiersze każdego esp-slave z ich ID i pomiarami
        interpretowane to będzie na serwerze
    3. protokół dołączania zrobić, gdy esp-slave nie ma w pamięci ID esp-master (lub ma jakiś bool ustawiony na false obok tego)
        to przy dołączaniu do sieci synchronizuje czas i 
        daje query że dołączył (na serwerze jest to interpretowane czy jest to nowy esp-slave czy też zmienił lokację [czy żadne z nich i 
        rozładowała mu się bateria po prostu])
    4. protokół synchronizowania czasu, gdy esp-slave mają za dużo rozbieżność od tego co myślą że powinno być i od tego co mają
        (można też po prostu zrobić żeby np co siódmy raz sobie synchronizowało i dostosywało czas sleepa +- żeby to było równo o godzinie o'clock)
        (można też po prostu zrobić żeby przy każdym wstaniu mierzyło co ma, wysyłało do esp-master i esp-master odpowiada że na ile sekund ma iść spać,
        to by w sumie rozwiązało protokół dołączania (nie licząc pierwszego pomiaru nie-o-tej-samej godzinie co inne),ale wtedy rozpoznawanie że esp-slave
        zmienił mastera/został dodany by musiał robić serwer (co w sumie jest słuszne))
    5. baza danych wraz ze wszystkimi tabelami
    6. funkcje php do obsługi wymienionych czynności zaimplementować
    7. front do strony
    8. używanie tych funkcji php ze strony (typu zmiany nazwy itp lokacji)
    9. wyświetlanie wszystkich wyników z różnymi atrakcjami, wykresy dla ula, od czasu tare, od danej daty, wykres ogólny wagowy albo przyrostu wagi od czasu
        tare albo ponad tym czasem i z pionowymi kreskami w punkcie tare
    10. opcjonalnie - śledzenie przeszłości esp-master i/lub esp-slave
    */
?>
