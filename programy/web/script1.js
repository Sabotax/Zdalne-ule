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
    xmlhttp.open("POST", "PHP/ajax.php?selector=" + selector + "&valueA="+valueA, true);
    
    xmlhttp.send();
}

function MyDebug(str,dopisac=false) {
    if(dopisac) {
        document.getElementById("debug").innerHTML+="<br>"+str;
    }
    else {
        document.getElementById("debug").innerHTML=str;
    }
    
}

let lista_pasieka = document.getElementById("lista_pasieka");
lista_pasieka.addEventListener("change",load_ule);