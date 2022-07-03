<?php
require_once("connect_data.php");
require_once("library.php");
header('Access-Control-Allow-Origin: *');
$mysqli = @new mysqli($host , $db_user, $db_password, $db_name);
?>