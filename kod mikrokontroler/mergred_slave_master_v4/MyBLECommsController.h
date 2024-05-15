String rozkaz = "";
bool rozkaz_done = false;
String data_incoming = "";

void decodeInput() {
  rozkaz = "";
  rozkaz_done = false;
  data_incoming = "";
  for(uint8_t i = 0; i < input_size; i++) {
    if(!rozkaz_done) {
      if(input_data[i] == '|') {
        rozkaz_done = true;
      }
      else {
        rozkaz += String( (char) input_data[i] );
      }
    }
    else {
      data_incoming += String( (char) input_data[i] );
    }
  }
  #ifdef DEBUG
    // input decoded
    Serial.println("rozkaz = " + rozkaz);
    Serial.println("dane = " + data_incoming);
  #endif
}

void rozkaz1() {
  // start sending data
  // TODO dekodowanie path?
  String path = "/"+data_incoming+".txt";
  #ifdef DEBUG
    Serial.println("Reading file: " + path);
  #endif

  file = SD.open(path);
  if(!file){
    String path2 = data_incoming+".txt";
    #ifdef DEBUG
      Serial.println("Failed to open file for reading");
      Serial.println("Reading file: " + path2);
    #endif
    file = SD.open(path);
    #ifdef DEBUG
      Serial.println("drugie otwarcie:" + bool(file));
    #endif
    myTXstring(5,"0");
  }
  else {
    myTXstring(1,"");
  }
  // TODO jak zrobić otwieranie pliku i wysyłanie pomiędzy kolejnymi pętlami
  // deklaracja wyżej i otwieranie zamykanie w 1 i 3 ?
}

void rozkaz2() {
  //continue sending data
  Serial.println("file "+ String((bool) file));
  if(file){
    String line = "";
    char c='#';
    
    while(file.available()){
      c = file.read();
      Serial.println(c);
    
      if(c == '#') {
        // file ended

        // TX with message(rozkaz) that file ended (and line)
        file.close();
        myTXrow(3,line);
        break;
      }
      else if(c == ';') {
        // line ended

        // TX with line and rozkaz that is ready to continue
        myTXrow(2,line);
        break;
      }
      else {
        line += String(c);
      }
    }

    if(!file.available()) {
      myTXstring(6,"");
      file.close();
    }
  }
}

void rozkaz4() {
  // send current weight
  wagaOdczyt = loadcell.get_units(2);
  #ifdef DEBUG
    Serial.println("wykonuje pomiar="+String(wagaOdczyt));
  #endif
  myTXstring(4,String(wagaOdczyt));
}

void rozkaz7() {
    // set offset to rtc and weight
  weight_offset = data_incoming.toFloat();
  loadcell.set_offset(weight_offset);
  myTXstring(7,"");
}

void rozkaz8() {
  // set scale to rtc and weight
  weight_scale = data_incoming.toFloat();
  loadcell.set_scale(weight_scale);
  myTXstring(8,"");
}

void rozkaz9() {
  // tare
  loadcell.tare();
  myTXstring(9,"");
}

void rozkaz10() {
  // TODO set okres przerw
}

bool executing_rozkaz11 = false;

void rozkaz11() {
  executing_rozkaz11 = true;
}

void execute_rozkaz11() {
  // todo opcjonalnie handshake z "AT" (tak samo dla signal)
  if ( getBattery() ) {
    myTXstring(11,String(batteryMeasure));
    batteryPobrane = false;
    rozkazWykonany_battery = false;
    executing_rozkaz11 = false;
  }
}

bool executing_rozkaz12 = false;

void rozkaz12() {
  executing_rozkaz12 = true;
}

void execute_rozkaz12() {
  if ( getSignal() ) {
    myTXstring(12,String(signalMeasure));
    rozkazWykonany_signal = false;
    signalPobrane = false;
    executing_rozkaz12 = false;
  }
}

void rozkazA() {
  uint32_t converted_epoch = data_incoming.toInt();
  setTimeRTC(converted_epoch);
}

void executeComms() {
  if(deviceConnected) {
    if(input_received) {
      #ifdef DEBUG
        Serial.println("Przetwarzam input");
      #endif
      input_received = false;

      decodeInput(); // todo przesunac do gory funkcje zeby byla widoczna

      #ifdef turnOnSD
        if(rozkaz == "1") rozkaz1();
        if(rozkaz == "2") rozkaz2();
      #endif
      
      #ifndef mockWeight
        if(rozkaz == "4") rozkaz4();
        if(rozkaz == "7") rozkaz7();
        if(rozkaz == "8") rozkaz8();
        if(rozkaz == "9") rozkaz9();
      #endif

      if(rozkaz == "10") rozkaz10();

      #ifdef GSM_turn_on
        if(rozkaz == "11") rozkaz11();
        if(rozkaz == "12") rozkaz12();
      #endif

      #ifndef mockRTC
        if(rozkaz == "A") rozkazA();
      #endif
    }

    if(executing_rozkaz11) {
      execute_rozkaz11();
    }
    if(executing_rozkaz12) {
      execute_rozkaz12();
    }
  }

  
}
