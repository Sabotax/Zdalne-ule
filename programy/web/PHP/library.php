<?php
function querySelect($mysqli,$query) {

    if($rezultat=$mysqli->query($query) ) {     //jeśli 1) uda się otrzymać wynik zapytania + 2) przypisuje go do rezultat
        $tablica = Array();
        if($rezultat->num_rows > 0) { // ??? jesli zapytanie nie jest puste - w kolejnosci bo u gory jeszcze by nie bylo przypisane/zadeklarowane
            for($j = 0 ; $row = mysqli_fetch_row($rezultat); $j++) { // tablica row, każda jej wartość to wiersz z tabeli z outputu

                for($i = 0; $i < count($row) ; $i++) {
                    $tablica[$j][$i] = $row[$i]; // wybieranie koncowe (wiele kolumn)
                }
            }
            $rezultat->free_result(); // uwalnianie rezulatu
        }
        // else {
        //     // nic nie zwrociło
        //     return false;
        // }
        return $tablica;
    }
    else {
        echo("Błąd kwerendy: " . $query);
        return false;
    }
}

function load_pasieki($mysqli) {
    if( !($mysqli -> connect_errno)) {
        $lista_esp_master = querySelect($mysqli,"SELECT `ID`,`nazwa` FROM `esp-master`");
        echo '<option value="lista_pasieka_select_puste"></option>';
        foreach($lista_esp_master as $row) {
            echo "<option value=\"$row[0]\">$row[1]</option>";
        }
    }
}


?>