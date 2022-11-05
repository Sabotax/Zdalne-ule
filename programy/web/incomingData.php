<?php
// if ($_SERVER['REQUEST_METHOD'] === 'POST') {
//     require_once("PHP/connect_init.php");
//     $input = file_get_contents('php://input');
//     $myfile = fopen("incoming_data-".date("Y-m-d_H-i-s").".txt", "w");
//     fwrite($myfile,$input);
//     fclose($myfile);
//     echo "jest w pyte";
// }

$token_autoryzujacy = "Watykanczyk2137";
$require_once("library.php");

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $incoming_data = json_decode(file_get_contents('php://input'));
    if(verifyIncomingData($incoming_data)) {
        require_once("PHP/connect_init.php");
        insertPomiar($incoming_data);
    }
}


?>
