async function load_chart() {
    let div_canvas = document.getElementById("center_tresc");
    div_canvas.innerHTML = '<canvas id="myChart" width="940" height="600"></canvas>';
    let ctx = document.getElementById('myChart').getContext('2d');
    //TODO temperatura i kolory, tło, oś y do temperatury po prawej
    let my_array = await get_data_for_chart()
    console.log(my_array)

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
    return new Promise(resolve => {
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                let my_array2 = JSON.parse(this.responseText)
                resolve(my_array2)
            }
        };
        let selector = "chart"
        let typ = document.getElementById("chart_typ").value
        let czas_od = document.getElementById("okres_czasu_input_od").value + " 23:59:59";
        let czas_do = document.getElementById("okres_czasu_input_do").value + " 00:00:00";
        let ul = document.getElementById("lista_ule").value;

        xmlhttp.open("POST", "PHP/ajax.php?selector=" + selector + "&typ="+typ+ "&czas_od="+czas_od+ "&czas_do="+czas_do+"&ul="+ul, true);
        
        xmlhttp.send();
    });
}

// function get_data_for_chart() {
//     let selector = "chart"
//     let typ = document.getElementById("chart_typ").value
//     let czas_od = document.getElementById("okres_czasu_input_od").value + " 23:59:59";
//     let czas_do = document.getElementById("okres_czasu_input_do").value + " 00:00:00";
//     let ul = document.getElementById("lista_ule").value;

//     return new Promise(function(resolve, reject) {
//       $.ajax({
//         url: 'PHP/ajax.php',
//         data: {selector: selector, typ: typ, czas_od: czas_od, czas_do: czas_do, ul: ul},
//         success: function(data) {
//           resolve(data) // Resolve promise and go to then()
//         },
//         error: function(err) {
//           reject(err) // Reject the promise and go to catch()
//         }
//       });
//     });
//   }

//   get_data_for_chart().then(function(data) {
//     // Run this when your request was successful
//     console.log(data)
//   }).catch(function(err) {
//     // Run this when promise was rejected via reject()
//     console.log(err)
//   })