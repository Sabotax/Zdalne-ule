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

        if( $input_data->client_api_key == server_api_key) {
            // jest przekaz od poprawnego naszego esp
            foreach( $input_data->tab as $ob ) {
                echo $ob->id," ",$ob->waga," ",$ob->temperatura,"\r\n" ;
            }
        }
    }
?>