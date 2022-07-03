function load_ule() { //str jaką wartość wysyła
// str to input a response text to output, a element(byid) to element edytowany
    //document.getElementById("center").setAttribute("background-color=\"#ccc\"");
    let selector = "load_ule";
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("lista_ule").innerHTML = this.responseText; // responseText - to to co będzie echo w ajax.php
        }
    };
    let valueA = document.getElementById("lista_pasieka").value

    //debug
    //document.getElementById("center").innerHTML=valueA;

    if (valueA == "lista_pasieka_select_puste") {
        //document.getElementById("center").innerHTML+="if";
        //document.getElementById("lista_ule").setAttribute("disabled",true);
        document.getElementById("lista_ule").disabled = true;
    }
    else {
        //document.getElementById("center").innerHTML+="else";
        //document.getElementById("lista_ule").setAttribute("disabled",false);
        document.getElementById("lista_ule").disabled = false;
    }
    let site = 'http://daniel.rozycki.student.put.poznan.pl/'
    xmlhttp.open("POST", site+"PHP/ajax.php?selector=" + selector + "&valueA="+valueA, true);
    
    xmlhttp.send();
}

function MyDebug(str,dopisac=false) {
    if(dopisac) {
        document.getElementById("topbar").innerHTML+="<br>"+str;
    }
    else {
        document.getElementById("topbar").innerHTML=str;
    }
    
}

let lista_pasieka = document.getElementById("lista_pasieka");
lista_pasieka.addEventListener("change",load_ule);

let lista_ule = document.getElementById("lista_ule");
lista_ule.addEventListener("change",load_opcje_chart);

let chart_btn;

function load_opcje_chart() {
    let div = document.getElementById("center_opcje");
    let time = new Date();
    let dzis = time.toISOString().split('T')[0]
    time.setDate(time.getDate() - 7);
    let tydzien_temu = time.toISOString().split('T')[0]

    div.innerHTML=`
    <P>
    Okres czasu: <input id="okres_czasu_input_od" type="date" value="${tydzien_temu}">
    - <input id="okres_czasu_input_do" type="date" value="${dzis}">
    Typ: 
    <select id="chart_typ">
        <option value="chart_typ_ogolny">Ogólna waga</option>
        <!-- <option value="chart_typ_roznicowy">Różnicowa waga</option> -->
    </select>
    <input id="chart_btn" type="button" value="Pokaż">
    </P>
    `
    chart_btn = document.getElementById("chart_btn");
    chart_btn.addEventListener("click",load_chart)
}