#define RX2 16
#define TX2 17

// Your GPRS credentials, if any
const char apn[]      = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

bool connectingSuccess = false;
#define DUMP_AT_COMMANDS
// TODO, jesli nie wykryje OK kilka razy to pomija calosc

unsigned long waitResponseTimeStart = 0;
bool waitResponseInitialized = false;
String response = "";

bool waitResponseAsync(uint8_t t) {
  if(!waitResponseInitialized) {
    waitResponseTimeStart = millis();
    waitResponseInitialized = true;
    response = "";
  }

  if(millis() >  waitResponseTimeStart + (1000 * t) ) {
    waitResponseInitialized = false;

    while( SerialAT.available() != 0 ) {
      response += (char) SerialAT.read();
    }
    response.trim();

    #ifdef DUMP_AT_COMMANDS 
      #ifndef GsmMockOn
        SerialMon.println("|"+response+"|");
      #endif
    #endif 
    
    return true;
  }
  else {
    return false;
  }
}

void sendAT(String s) {
  #ifdef DUMP_AT_COMMANDS
    Serial.println("Wysylanie komendy AT: " + s);
  #endif
  
  SerialAT.println(s);
}

bool czekamNaOdpowiedzGSM = false;

bool sendCommandGSM(String command, uint8_t responseWaitTime, String &responseMsg, bool &rozkazWykonany) {
  if(!czekamNaOdpowiedzGSM) {
    sendAT(command);
    czekamNaOdpowiedzGSM = true;
    return false;
  }
  else {
    bool gotResponse = waitResponseAsync(responseWaitTime);
    if(gotResponse) {
      #ifdef GsmMockOn
        if(command == "AT+CREG?") responseMsg = "+CREG: 0,1\r\n\r\nOK";
        #ifdef GsmPostMockOn
          else if(command == "AT+HTTPINIT") responseMsg = "OK";
        #endif
        else responseMsg = "default_mock_msg";

        SerialMon.println("Symuluje otrzymanie odpowiedzi: " + responseMsg);
      #else  
        responseMsg = response;
      #endif
      
      rozkazWykonany = gotResponse;
      czekamNaOdpowiedzGSM = false;
    }
    return gotResponse;
  }
}


// progress

// reset Sim

// isConnected

bool rozkazWykonany_battery = false;
String odp_battery = "";
int batteryMeasure = -1;
bool batteryPobrane = false;

bool getBattery() { // todo reset flag na koniec sekwencji
  #ifdef DEBUG
    if(!rozkazWykonany_battery) Serial.println(F("getBattery krok 0"));
  #endif
  
  if( batteryPobrane ) return true;
  
  if(!rozkazWykonany_battery) sendCommandGSM("AT+CBC",2,odp_battery, rozkazWykonany_battery);
  if( rozkazWykonany_battery ) {
    int startIndex = odp_battery.indexOf(',') + 1;
    int endIndex = odp_battery.indexOf(',', startIndex);
    String secondNumberString = odp_battery.substring(startIndex, endIndex);
    batteryMeasure = secondNumberString.toInt();

    #ifdef DEBUG
      Serial.println("Get battery, extracted number " + secondNumberString);
    #endif
    batteryPobrane = true;
    return true;
  }
  return false;
}

bool rozkazWykonany_signal = false;
String odp_signal = "";
int signalMeasure = -1;
bool signalPobrane = false;

bool getSignal() { // todo reset flag na koniec sekwencji
  #ifdef DEBUG
    if(!rozkazWykonany_signal) Serial.println(F("getSignal krok 0"));
  #endif

  if(signalPobrane) return true;
  
  sendCommandGSM("AT+CSQ",2,odp_signal, rozkazWykonany_signal);
  if( rozkazWykonany_signal ) {
    int startIndex = odp_signal.indexOf(':') + 2;
    int endIndex = odp_signal.indexOf(',', startIndex);
    
    String value = odp_signal.substring(startIndex, endIndex);
    signalMeasure = value.toInt();
    #ifdef DEBUG
      Serial.println("Get signal, extracted number " + value);
    #endif
    signalPobrane = true;
    return true;
  }
  return false;
}

// todo get Http status

// todo handshake

bool rozkazWykonany_restart[2] = {false};
String odp_restart[2] = {""};

bool restartGSM() { // todo upewnic sie, czy po restarcie/w ramach retry-post nie trzeba tez zainicjowac sima
  #ifdef DEBUG
    Serial.print(F("\nrestartGSM "));
    if(!rozkazWykonany_restart[0]) Serial.print(F(" 0"));
  #endif
  if(!rozkazWykonany_restart[0]) sendCommandGSM("AT+CFUN=0",3,odp_restart[0], rozkazWykonany_restart[0]);
  else {
    #ifdef DEBUG
      if(!rozkazWykonany_restart[1]) Serial.print(F("1"));
    #endif
    if(!rozkazWykonany_restart[1]) sendCommandGSM("AT+CFUN=1,1",17,odp_restart[1], rozkazWykonany_restart[1]);
    else {
      for(uint8_t i = 0; i < 2; i++) {
        rozkazWykonany_restart[i] = false;
      }
      #ifdef DEBUG
        Serial.println();
      #endif
      return true;
    }
  }
  #ifdef DEBUG
    Serial.println();
  #endif
  return false;
}

bool rozkazWykonany_init[7] = {false};
String odp_init[7] = {""};

bool initGSM() { // zwraca bool na potrzeby uruchomienia po restarcie/włączeniu w trybie blocking (w pętli while)
  #ifdef DEBUG
    Serial.print(F("initGSM "));
    if(!rozkazWykonany_init[0]) Serial.print(F("0"));
  #endif
  if(!rozkazWykonany_init[0]) sendCommandGSM("AT",2,odp_init[0], rozkazWykonany_init[0]);
  if( rozkazWykonany_init[0] ) {
    #ifdef DEBUG
      if(!rozkazWykonany_init[1]) Serial.print(F("1"));
    #endif
    if(!rozkazWykonany_init[1]) sendCommandGSM("AT+CREG?",1,odp_init[1], rozkazWykonany_init[1]);
    if( rozkazWykonany_init[1] ) {
      #ifdef DEBUG
        if(!rozkazWykonany_init[2]) Serial.print(F("2"));
      #endif
      if(odp_init[1] == "+CREG: 0,1\r\n\r\nOK") { // isConnected
        if(!rozkazWykonany_init[2]) sendCommandGSM("AT+SAPBR=3,1,\"Contype\",\"GPRS\"",1,odp_init[2], rozkazWykonany_init[2]); // todo? da sie polaczyc obie komendy?
        if( rozkazWykonany_init[2] ) {
          if(odp_init[2] == "ERROR") {
            #ifdef DEBUG
              Serial.println(F("initGSM gsm connection ERROR"));
            #endif
            // todo? reset sim i od nowa przez <retry> ilosc razy
            //temp
            return true;
          }
          else {
            #ifdef DEBUG
              if(!rozkazWykonany_init[3]) Serial.print(F("3"));
            #endif
            if(!rozkazWykonany_init[3])sendCommandGSM("AT+SAPBR=3,1,\"APN\",\"internet\"",1,odp_init[3], rozkazWykonany_init[3]);
            if( rozkazWykonany_init[3] ) {
              #ifdef DEBUG
                if(!rozkazWykonany_init[4]) Serial.print(F("4"));
              #endif
              if(!rozkazWykonany_init[4]) sendCommandGSM("AT+SAPBR=3,1,\"USER\",\"\"",1,odp_init[4], rozkazWykonany_init[4]);
              if( rozkazWykonany_init[4] ) {
                #ifdef DEBUG
                  if(!rozkazWykonany_init[5]) Serial.print(F("5"));
                #endif
                if(!rozkazWykonany_init[5]) sendCommandGSM("AT+SAPBR=3,1,\"PWD\",\"\"",1,odp_init[5], rozkazWykonany_init[5]);
                if( rozkazWykonany_init[5] ) {
                  #ifdef DEBUG
                    if(!rozkazWykonany_init[6]) Serial.print(F("6"));
                  #endif
                  if(!rozkazWykonany_init[6]) sendCommandGSM("AT+SAPBR=1,1",1,odp_init[6], rozkazWykonany_init[6]);
                  if( rozkazWykonany_init[6] ) {
                    #ifdef DEBUG
                      Serial.println(F(" initGSM finish"));
                    #endif
                    // finish
                    for(uint8_t i = 0; i < 7; i++) {
                      rozkazWykonany_init[i] = false;
                    }
                    return true;
                  }
                }
              }
            }
          }
        }
      }
      else {
        // todo not connected error? cos zrobic/pokazac
        // nie ma zasiegu? -> nie wysylac?
        //temp
        return true;
      }
    }
  }
  #ifdef DEBUG
    Serial.println();
  #endif
  return false;
}

uint16_t wytnijStatus(String input) {
  int startIndex = input.indexOf(',') + 1;
  int endIndex = input.indexOf(',', startIndex);
  
  String value = input.substring(startIndex, endIndex);
  int valueNumber = value.toInt();
  #ifdef DEBUG
    SerialMon.println("\nExtracted value status number: " + value);
  #endif

  return valueNumber;
}

bool rozkazWykonany_post[9] = {false};
String odp_post[9] = {""};
uint8_t retryCounter = 0;
bool wytnijStatusOnce = false;
int httpStatus = 0;

void clearAllFlagsPostGsm() {
  for(uint8_t i = 0; i < 9; i++) {
    rozkazWykonany_post[i] = false;
  }
  batteryPobrane = false;
  rozkazWykonany_battery = false;
  sendingData = false;
  dataToSendCreated = false;
  retryCounter = 0;
  wytnijStatusOnce = false;
}

void makePostGSM() {

  #ifdef DEBUG
    Serial.print(F("postGSM "));
    if(!rozkazWykonany_post[0]) Serial.print(F("0"));
  #endif
  if(!rozkazWykonany_post[0]) sendCommandGSM("AT",2,odp_post[0], rozkazWykonany_post[0]);
  if( rozkazWykonany_post[0] ) {
    #ifdef DEBUG
      if(!rozkazWykonany_post[1]) Serial.print(F("1"));
    #endif
    if(!rozkazWykonany_post[1]) sendCommandGSM("AT+HTTPINIT",2,odp_post[1], rozkazWykonany_post[1]);
    if( rozkazWykonany_post[1] ) {
      if( odp_post[1] != "OK" ) {
        // błąd, retry
        #ifdef DEBUG
          Serial.print("2 - blad, retry " + String(retryCounter));
        #endif

        if(retryCounter < 1) {
          if(restartGSM()) {
            // jak clearAllFlags, ale tylko czysci te potrzebne do zrestartowania sekwencji
            for(uint8_t i = 0; i < 9; i++) {
              rozkazWykonany_post[i] = false;
            }
            retryCounter++;
          }
        }
        else {
          #ifdef DEBUG
            Serial.print(F(", zarzucam post"));
          #endif
          clearAllFlagsPostGsm();
        }

      }
      else {
        #ifdef DEBUG
          if(!rozkazWykonany_post[2]) Serial.print(F("2"));
        #endif
        if(!rozkazWykonany_post[2]) sendCommandGSM("AT+HTTPPARA=\"URL\",\""+serverAndRes+"\"",2,odp_post[2], rozkazWykonany_post[2]);
        if( rozkazWykonany_post[2] ) {
          #ifdef DEBUG
            if(!rozkazWykonany_post[3]) Serial.print(F("3"));
          #endif
          if(!rozkazWykonany_post[3]) sendCommandGSM("AT+HTTPPARA=\"CONTENT\",\"application/json\"",2,odp_post[3], rozkazWykonany_post[3]);
          if( rozkazWykonany_post[3] ) {
            #ifdef DEBUG
              if(!rozkazWykonany_post[4]) Serial.print(F("4"));
            #endif
            if(!rozkazWykonany_post[4]) sendCommandGSM("AT+HTTPDATA=" + String(dataToSend.length()) + ",100000",2,odp_post[4], rozkazWykonany_post[4]);
            if( rozkazWykonany_post[4] ) {
              #ifdef DEBUG
                if(!rozkazWykonany_post[5]) Serial.print(F("5"));
              #endif
              if(!rozkazWykonany_post[5]) sendCommandGSM(dataToSend,2,odp_post[5], rozkazWykonany_post[5]);
              if( rozkazWykonany_post[5] ) {
                #ifdef DEBUG
                  if(!rozkazWykonany_post[6]) Serial.print(F("6"));
                #endif
                if(!rozkazWykonany_post[6]) sendCommandGSM("AT+HTTPACTION=1",15,odp_post[6], rozkazWykonany_post[6]);
                if( rozkazWykonany_post[6] ) {
                  // todo akcja na odp_post[6] (wyciecie http status)
                  // jesli inny niz 200 to retry? albo zarzucenie (reset sima) i retry
                  
                  if(!wytnijStatusOnce) {
                    httpStatus = wytnijStatus(odp_post[6]);
                    wytnijStatusOnce = true;
                  }

                  if(httpStatus != 200) {
                    if(retryCounter < 2) {
                      if(restartGSM()) {
                        // jak clearAllFlags, ale tylko czysci te potrzebne do zrestartowania sekwencji
                        for(uint8_t i = 0; i < 9; i++) {
                          rozkazWykonany_post[i] = false;
                        }
                        wytnijStatusOnce = false;
                        retryCounter++;
                      }
                    }
                    else {
                      #ifdef DEBUG
                        Serial.print(F(", zarzucam post"));
                      #endif
                      clearAllFlagsPostGsm();
                    }
                  }
                  else {
                    #ifdef DEBUG
                      if(!rozkazWykonany_post[7]) Serial.print(F("7"));
                    #endif
                    if(!rozkazWykonany_post[7]) sendCommandGSM("AT+HTTPREAD",2,odp_post[7], rozkazWykonany_post[7]);
                    if( rozkazWykonany_post[7] ) {
                      // todo w tym miejcu mamy response w odp_post7
                      #ifdef DEBUG
                        if(!rozkazWykonany_post[8]) Serial.print(F("8"));
                      #endif
                      if(!rozkazWykonany_post[8]) sendCommandGSM("AT+HTTPTERM",2,odp_post[8], rozkazWykonany_post[8]);
                      
                      clearAllFlagsPostGsm();
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  #ifdef DEBUG
    Serial.println();
  #endif
}
