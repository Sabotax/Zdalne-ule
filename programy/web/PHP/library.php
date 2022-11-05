<?php
function querySelect($mysqli,$query) {

    if ($mysqli->connect_error) {
        die("Connection failed: " . $mysqli->connect_error);
    }

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

function insertPojedynczy($mysqli,$input,$zbiorowy_id) {
    if ($mysqli->connect_error) {
        die("Connection failed: " . $mysqli->connect_error);
    }

    //TODO for each row pojedynczych
    
        $query = "
        INSERT INTO `pomiary_pojedyncze`(`ID_pomiar_zbiorowy`, `ID_esp_slave`, `waga`, `temperatura`) VALUES ($zbiorowy_id,'[value-3]','[value-4]','[value-5]')
        ";

    if ($mysqli->query($sql) === TRUE) {
    echo "New record created successfully";
    } else {
    echo "Error: " . $sql . "<br>" . $conn->error;
    }
      
    //$mysqli->close();
}

function insertKilkaPojedynczych($mysqli,$input,$zbiorowy_id) {
    if ($mysqli->connect_error) {
        die("Connection failed: " . $mysqli->connect_error);
    }

    for($i=0; $i < count($input["data"]) ; $i++ ) {
        $query = "
        INSERT INTO `pomiary_pojedyncze`(`ID_pomiar_zbiorowy`, `ID_esp_slave`, `waga`, `temperatura`) 
        VALUES ($zbiorowy_id,$input[data][$i][S],$input[data][$i][W],$input[data][$i][I])
        ";

        if ($mysqli->query($sql) === TRUE) {
            echo "New record pojedynczy $i created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $mysqli->error;
        }
    }

    if ($mysqli->query($sql) === TRUE) {
    echo "New record created successfully";
    } else {
    echo "Error: " . $sql . "<br>" . $mysqli->error;
    }
      
    //$mysqli->close();
}

function insertZbiorowy($mysqli,$input) {
    $query = "INSERT INTO `pomiary_zbiorowe`(`ID_esp-master`, `data`, `temperatura_zewn`) VALUES ($input[M],$input[A],$input[C])";
    $last_id = -1;

    if ($mysqli->connect_error) {
        die("Connection failed: " . $mysqli->connect_error);
    }

    if ($mysqli->query($sql) === TRUE) {
    $last_id = $mysqli->insert_id;
    echo "New record created successfully";
    } else {
    echo "Error: " . $sql . "<br>" . $mysqli->error;
    }
          
    //$mysqli->close();
    return $last_id;
}

function insertPomiar($mysqli,$input) {
    $id = insertZbiorowy($mysqli,$input);
    insertKilkaPojedynczych($mysqli,$input,$id);
}

function verifyIncomingData($incomingData) {
    $token_autoryzujacy = "Watykanczyk2137";
    if($incoming_data["T"] == $token_autoryzujacy) {
        return true;
    }
    return false;
}

?>