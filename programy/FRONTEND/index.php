<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title></title>
        <meta name="description" content="">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="style.css">
    </head>
    <body>
        <div id="container">
            <div id="top">
                <div id="topbar">
                    bars
                </div>
            </div>
            <div id="left">
                <div id="pasieka">
                    <div id="pasieka_select">
                        <p>Wybór pasieki</p>
                        <select id="lista_pasieka">
                            <?php
                            // wszytkie esp-master z bazy
                            ?>
                        </select>
                        <button id="btn_pasieka_pokaz" disabled>Pokaż</buton>
                    </div>
                    <div id="pasieka_opcje">
                        <button id="btn_pasieka_zmien_nazwe" disabled>Zmień nazwę</button>
                    </div>
                </div>
                <div id="ul">
                    <div id="ul_select">
                        <p>Wybór ula</p>
                        <select id="lista_ule" disabled>
                            <?php
                            // wszytkie esp-slave dla danego esp-master z bazy
                            ?>
                        </select>
                        <button id="btn_ule_pokaz" disabled>Pokaż</buton>
                    </div>
                    <div id="ul_opcje">
                        <button id="btn_ul_zmien_nazwe" disabled>Zmień nazwę</button>
                    </div>
                </div>
                <div id="ul_showcase">
                    
                </div>
            </div>
            <div id="center">
            </div>
        </div>
        <script src="script.js" async defer></script>
    </body>
</html>