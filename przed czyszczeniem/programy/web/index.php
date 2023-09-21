<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title></title>
        <meta name="description" content="">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.8.0/chart.min.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/chartjs-adapter-date-fns/dist/chartjs-adapter-date-fns.bundle.min.js"></script>
        <link rel="stylesheet" href="style.css">
    </head>
    <body>
        <?php
            require_once("PHP/connect_init.php");
        ?>
        <div id="container">
            <div id="top">
                <div id="topbar">
                    
                </div>
            </div>
            <div id="left">
                <div id="ul">
                    <div id="ul_select">
                        <p>Wybór ula</p>
                        <select id="lista_ule">
                            <?php
                            //include_once("connect_init.php");
                            // wszytkie esp-slave dla danego esp-master z bazy
                            ?>
                        </select>
                        <button id="btn_ule_pokaz">Pokaż</buton>
                    </div>
                </div>
                <div id="ul_showcase">
                    
                </div>
            </div>
            <div id="center">
                <div id="center_opcje">
                </div>
                <div id="center_tresc">
                    <?php
                    ?>
                </div>
            </div>
            <div id="stopka">
                Różycki
            </div>
        </div>
        <script type="text/javascript" src="script_chart.js"></script>
        <script type="text/javascript" src="script1.js"></script>
    </body>
</html>