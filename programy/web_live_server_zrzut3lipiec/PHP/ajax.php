<?php
$http_origin = $_SERVER['HTTP_ORIGIN'];

$allowed_domains = array(
  'http://daniel.rozycki.student.put.poznan.pl',
);

if (in_array($http_origin, $allowed_domains))
{  
    header("Access-Control-Allow-Origin: $http_origin");
}

require_once("connect_init.php");
require_once("library.php");
if( !( $mysqli->connect_errno ) ) { //jesli sie uda polaczyć
    $selector = $_REQUEST["selector"];
    
    if ($selector == "load_ule" ) {
        $lista_pasieki_value = $_REQUEST["valueA"];

        if($lista_pasieki_value=="lista_pasieki_select_puste") {
            echo '<option value="lista_ule_select_puste"></option>';
        }
        else {
            $ule_respond = querySelect($mysqli,"SELECT `ID`,`nazwa` FROM `esp-slave` WHERE `ID_esp-master`=$lista_pasieki_value");
    
            echo '<option value="lista_ule_select_puste"></option>';
            foreach($ule_respond as $row) {
                echo "<option value=\"$row[0]\">$row[1]</option>";
            }
        }
    }

    if ($selector == "chart") {
        $typ = $_REQUEST["typ"];
        $czas_od = $_REQUEST["czas_od"];
        $czas_do = $_REQUEST["czas_do"];
        $ul = $_REQUEST["ul"];
        
        $pomiary_respond = querySelect($mysqli,
        "SELECT pomiary_pojedyncze.waga,pomiary_pojedyncze.temperatura,pomiary_zbiorowe.data,pomiary_zbiorowe.temperatura_zewn
        FROM pomiary_pojedyncze INNER JOIN pomiary_zbiorowe
        ON pomiary_pojedyncze.ID_pomiar_zbiorowy=pomiary_zbiorowe.ID_pomiar_zbiorowy
        WHERE pomiary_pojedyncze.ID_esp_slave=$ul
        AND (pomiary_zbiorowe.data BETWEEN '$czas_od' AND '$czas_do')");
        
        /*
        Array ( 
            [0] => Array ( 
                [0] => 35.00 [1] => 25.00 [2] => 2022-07-02 20:39:09 [3] => 25.00 
                ) 
            [1] => Array ( 
                [0] => 45.00 [1] => 35.00 [2] => 2022-07-02 21:39:14 [3] => 22.00 
                )
            )
        */

        // foreach($pomiary_respond as $row) {
        //     echo "$row[0]" . "," . "$row[1]" . "$row[2]" . "$row[3]" . ";";
        // }
        echo json_encode($pomiary_respond);
    }
    
}


?>