function load_chart() {
    let div_canvas = document.getElementById("center_tresc");
    div_canvas.innerHTML = '<canvas id="myChart" width="940" height="600"></canvas>';
    let ctx = document.getElementById('myChart').getContext('2d');

    //TODO temperatura i kolory, tło, oś y do temperatury po prawej

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

function get_data_for_chart(czy_roznicowy,od_kiedy) {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //document.getElementById("lista_ule").innerHTML = this.responseText; // responseText - to to co będzie echo w ajax.php
            // zwrócić this.responseText ale sformatowany odpowiednio pod chart.js
        }
    };
    let selector = "chart"
    let typ = document.getElementById("chart_typ").value
    let czas_od = document.getElementById("okres_czasu_input_od").value;
    let czas_do = document.getElementById("okres_czasu_input_do").value;

    xmlhttp.open("POST", "PHP/ajax.php?selector=" + selector + "&typ="+typ+ "&czas_od="+czas_od+ "&czas_do="+czas_do, true);
    
    xmlhttp.send();
}