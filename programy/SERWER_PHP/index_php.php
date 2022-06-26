<?php
    /* input structure
    object
        client_api_key = xxx
        data = np. 26.06.2022
        tab = Array
            object[0]
                id
                waga
                temperatura
            object[1]
            itp, wielkość nieznana, dynamiczna
    -----
    do pól obiektu odwołanie: ob->pole
    */
    define("server_api_key","xxx");

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $json = file_get_contents('php://input');
        $input_data = json_decode($json);
        //print_r($data);

        require_once("connect.php");
        require_once("library.php");

        // IF jest przekaz od poprawnego naszego esp
        if( $input_data->client_api_key == server_api_key) {
            $polaczenie = @new mysqli($host , $db_user, $db_password, $db_name);

            /* unikatowe insert
            INSERT INTO `pomiary`(`nr_ula`,`pasieka`,`data`,`waga`,`temperatura`)
                SELECT 15,"u roberta",'2022-06-27 22:00:36',5.6,27.3
            WHERE NOT EXISTS ( 
                SELECT `nr_ula`,`pasieka`,`data`,`waga`,`temperatura` FROM `pomiary` 
                WHERE `nr_ula`=15 AND `pasieka`="u roberta" AND `data`='2022-06-27 22:00:36' AND `waga` = 5.6 AND `temperatura`=27.3
            );
            */
            
            foreach( $input_data->tab as $ob ) {
                $query = "INSERT INTO `$db_name`(`nr_ula`,`pasieka`,`data`,`waga`,`temperatura`) SELECT $ob->id,$input_data->pasieka,'$input_data->data',$ob->waga,$ob->temperatura WHERE NOT EXISTS ( SELECT `nr_ula`,`pasieka`,`data`,`waga`,`temperatura` FROM `pomiary` WHERE `nr_ula`=$ob->id AND `data`='$ob->data' AND `waga` = $ob->waga AND `temperatura`=$ob->temperatura')";
                $rezultat = @$polaczenie->query($query);
                echo $rezultat , "<br>\r\n";
            }
        }
    }
?>