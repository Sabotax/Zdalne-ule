function load_chart() {
    let div_canvas = document.getElementById("center_tresc");
    div_canvas.innerHTML = '<canvas id="myChart" width="940" height="600"></canvas>';
    let ctx = document.getElementById('myChart').getContext('2d');
    //TODO temperatura i kolory, tło, oś y do temperatury po prawej
    get_data_for_chart()

    let wykresChart = new Chart(ctx, {
        type:'line', // bar, horizontalBar,pie,line,doughnut,radar,polarArena
        data: {
            //labels:[ ],
            datasets:[{
                label: 'Waga',
                data: [ {x:'2016-10-26 20:00', y:'3000'} , {x:'2016-12-27', y:'4000'},{x:'2016-12-28', y:'2500'}]
            }]
        },
        options: {
            scales: {
                xAxis: {
                // The axis for this scale is determined from the first letter of the id as `'x'`
                // It is recommended to specify `position` and / or `axis` explicitly.
                type: 'time',
                }
            }
        }
    });
}

function get_data_for_chart() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //document.getElementById("lista_ule").innerHTML = this.responseText; // responseText - to to co będzie echo w ajax.php
            // zwrócić this.responseText ale sformatowany odpowiednio pod chart.js
            document.getElementById("topbar").innerHTML = "test"+this.responseText;
            // TODO return this.responseText ale jako gotowy array w js ^
            // TODO zobaczyć czemu nie response text = ""
        }
    };
    let selector = "chart"
    let typ = document.getElementById("chart_typ").value
    let czas_od = document.getElementById("okres_czasu_input_od").value + " 23:59:59";
    let czas_do = document.getElementById("okres_czasu_input_do").value + " 00:00:01";
    let ul = document.getElementById("lista_ule").value;

    xmlhttp.open("POST", "PHP/ajax.php?selector=" + selector + "&typ="+typ+ "&czas_od="+czas_od+ "&czas_do="+czas_do+"&ul="+ul, true);
    
    xmlhttp.send();
}
