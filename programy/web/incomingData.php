<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title></title>
        <meta name="description" content="">
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body>
        <?php
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            require_once("PHP/connect_init.php");
            $input = file_get_contents('php://input');
            $myfile = fopen("incoming_data-".date("Y-m-d_H-i-s").".txt", "w");
            fwrite($myfile,$input);
            fclose($myfile);
            echo "OK";
        }
        ?>
    </body>
</html>
