<?php
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

        //TODO
        $pomiary_respond = querySelect($mysqli,"");
    }
    
}


?>