<?php
    require_once("connect.php");
    require_once("library.php");

    $polaczenie = @new mysqli($host , $db_user, $db_password, $db_name);

    $tab = getSelectResponse(false,"SELECT * FROM `pomiary`",$polaczenie);

   // print_r($tab);

    echo "<table>";
    foreach($tab as $row) {
        echo "<tr>";
        foreach($row as $cell) {
            echo "<td>",$cell,"</td>";
        }
        echo "</tr>";
    }
    echo "</table>";
?>