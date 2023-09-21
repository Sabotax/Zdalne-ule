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

    xmlhttp.open("POST", "PHP/ajax.php?selector=" + selector, true);
    
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


let lista_ule = document.getElementById("lista_ule");
load_ule();

let btn_ule_pokaz = document.getElementById("btn_ule_pokaz");
btn_ule_pokaz.addEventListener("click", load_opcje_chart);

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