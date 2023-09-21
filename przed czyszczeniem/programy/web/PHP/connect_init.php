<?php
require_once("connect_data.php");
require_once("library.php");

$mysqli = @new mysqli($host , $db_user, $db_password, $db_name);
?>