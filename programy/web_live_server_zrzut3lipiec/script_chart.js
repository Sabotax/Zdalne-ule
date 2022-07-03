async function load_chart() {
    let div_canvas = document.getElementById("center_tresc");
    div_canvas.innerHTML = '<canvas id="myChart" width="940" height="600"></canvas>';
    let ctx = document.getElementById('myChart').getContext('2d');
    //TODO temperatura i kolory, tło, oś y do temperatury po prawej
    let my_array = await get_data_for_chart()

    // dostosowanie do chart
    let waga_array = Array();
    let temperatura_array = Array();
    let temperatura_zewn_array = Array();

    my_array.forEach(element => {
        waga_array.push(
            {x: element[2],y: element[0]}
        )
        temperatura_array.push(
            {x: element[2],y: element[1]}
        )
        temperatura_zewn_array.push(
            {x: element[2],y: element[3]}
        )
    });

    let wykresChart = new Chart(ctx, {
        type:'line', // bar, horizontalBar,pie,line,doughnut,radar,polarArena
        data: {
            //labels:[ ],
            datasets:[
                {
                    label: 'Waga',
                    data: waga_array,
                    borderColor: '#0000ff'
                },
                {
                    label: 'Temperatura w ulu',
                    data: temperatura_array,
                    borderColor: '#ff3333'
                },
                {
                    label: 'Temperatura zewnętrzna',
                    data: temperatura_zewn_array,
                    borderColor: '#ff8000'
                }
            ]
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
    return new Promise(output => {
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                console.log(this.responseText)
                let my_array2 = JSON.parse(this.responseText)
                
                output(my_array2)
            }
        };
        let selector = "chart"
        let typ = document.getElementById("chart_typ").value
        let czas_od = document.getElementById("okres_czasu_input_od").value + " 23:59:59";
        let czas_do = document.getElementById("okres_czasu_input_do").value + " 00:00:00";
        let ul = document.getElementById("lista_ule").value;

        let site = 'http://daniel.rozycki.student.put.poznan.pl/'
        xmlhttp.open("POST", site+"PHP/ajax.php?selector=" + selector + "&typ="+typ+ "&czas_od="+czas_od+ "&czas_do="+czas_do+"&ul="+ul, true);
        
        xmlhttp.send();
    });
}

