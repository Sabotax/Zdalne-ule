<?php
$http_origin = $_SERVER['HTTP_ORIGIN'];

$allowed_domains = array(
  'http://daniel.rozycki.student.put.poznan.pl',
  'http://localhost'
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

        $ule_respond = querySelect($mysqli,"SELECT `ID`,`nazwa` FROM `esp_devices`");

        foreach($ule_respond as $row) {
            echo "<option value=\"$row[0]\">$row[1]</option>";
        }

    }

    if ($selector == "chart") {
        $typ = $_REQUEST["typ"];
        $czas_od = $_REQUEST["czas_od"];
        $czas_do = $_REQUEST["czas_do"];
        $ul = $_REQUEST["ul"];
        $timeFlag = false;
        if($_REQUEST["timeFlag"] == "true") $timeFlag=true;
        
        // $pomiary_respond = querySelect($mysqli,
        // "SELECT pomiary_pojedyncze.waga,pomiary_pojedyncze.temperatura,pomiary_zbiorowe.data,pomiary_zbiorowe.temperatura_zewn
        // FROM pomiary_pojedyncze INNER JOIN pomiary_zbiorowe
        // ON pomiary_pojedyncze.ID_pomiar_zbiorowy=pomiary_zbiorowe.ID_pomiar_zbiorowy
        // WHERE pomiary_pojedyncze.ID_esp_slave=$ul
        // AND (pomiary_zbiorowe.data BETWEEN '$czas_od' AND '$czas_do')");

        $query = "SELECT pomiary.waga,pomiary.timeFromEsp,pomiary.bateria
        FROM pomiary
        WHERE pomiary.espID='esp01' ";

        if($timeFlag) $query.= "AND (pomiary.timeFromDB BETWEEN '$czas_od' AND '$czas_do')";
        else $query.= "AND (pomiary.timeFromEsp BETWEEN '$czas_od' AND '$czas_do')";

        $pomiary_respond = querySelect($mysqli,$query);



        // TODO zmiana id na dynamiczne
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