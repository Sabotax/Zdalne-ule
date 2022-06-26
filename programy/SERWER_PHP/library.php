<?php
//polaczenie i koniec polaczenia a takze errory jesli nie uda sie polaczyc poza tym!!
function getSelectResponse($switch,$query,$polaczenie) {
    //switch
    // true - 2D
    // false - 3D
    if($rezultat=@$polaczenie->query($query) ) {     //jeśli 1) uda się otrzymać wynik zapytania + 2) przypisuje go do rezultat
        $tablica = Array();
        if($rezultat->num_rows > 0) { // ??? jesli zapytanie nie jest puste - w kolejnosci bo u gory jeszcze by nie bylo przypisane/zadeklarowane
            
            
            for($j = 0 ; $row = mysqli_fetch_row($rezultat); $j++) { // tablica row, każda jej wartość to wiersz z tabeli z outputu

                if($switch) {
                    $tablica[$j] = $row[0]; // zwykle 2d jak np show databases
                }
                else {
                    for($i = 0; $i < count($row) ; $i++) {
                        $tablica[$j][$i] = $row[$i]; // wybieranie koncowe (wiele kolumn)
                    }
                } 
                
                
                //print_r($row);
                //print "<br>";
                /*
                for($i = 0;$i < count($row) ; $i++) { // normalne wyświetlanie zawartości tablicy
                    print $row[$i]."<br>";
                    //$tablica[$i] = $row[$i];
                }
                */
                //echo "<br>";
                
            }
            $rezultat->free_result(); // uwalnianie rezulatu
            //return $tablica;
        }
        else {
            //echo "Pusto";
            //return "Ni ma";
        }
        return $tablica;
    }
    //else echo "BŁĄD KWERENDY: ".$query;
}
?>