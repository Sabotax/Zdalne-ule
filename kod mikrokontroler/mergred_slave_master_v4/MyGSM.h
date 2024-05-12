#define RX2 16
#define TX2 17

#define SerialMon Serial
#define SerialAT Serial2

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

bool waitResponseAsync(uint8_t t = 1) {
  if(!waitResponseInitialized) {
    waitResponseTimeStart = millis();
    waitResponseInitialized = true;
    response = "";
    #ifdef
      SerialMon.println("Moment rozpoczęcia czekania: " + String(waitResponseTimeStart/1000));
    #endif
  }
  #ifdef
    SerialMon.println("Async response, czekam: " + String(millis()) + " | " + waitResponseTimeStart + (1000 * t)); // todo tutaj debug czemu nie odpowiada
  #endif

  if(millis() >  waitResponseTimeStart + (1000 * t) ) {
    waitResponseInitialized = false;

    while( SerialAT.available() != 0 ) {
      response += (char) SerialAT.read();
    }
    response.trim();

    #ifdef DUMP_AT_COMMANDS
      SerialMon.println("|"+response+"|");
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
  }
  else {
    bool gotResponse = waitResponseAsync(responseWaitTime);
    if(gotResponse) {
      // TODO MOCK
      
      responseMsg = response;
      rozkazWykonany = gotResponse;
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
    Serial.println(F("getBattery krok 0"));
  #endif
  
  if( batteryPobrane ) return true;
  
  sendCommandGSM("AT+CBC",2,odp_battery, rozkazWykonany_battery);
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
    Serial.println(F("getSignal krok 0"));
  #endif
  sendCommandGSM("AT+CSQ",2,odp_signal, rozkazWykonany_signal);
  if( rozkazWykonany_signal ) {
    int startIndex = odp_signal.indexOf(':') + 2;
    int endIndex = odp_signal.indexOf(',', startIndex);
    
    String value = odp_signal.substring(startIndex, endIndex);
    signalMeasure = value.toInt();
    #ifdef DEBUG
      Serial.println("Get signal, extracted number " + value);
    #endif
    return true;
  }
  return false;
}

// get Http status

// handshake

bool rozkazWykonany_init[7] = {false};
String odp_init[7] = {""};

bool initGSM() { // zwraca bool na potrzeby uruchomienia po restarcie/włączeniu w trybie blocking (w pętli while)
  #ifdef DEBUG
    Serial.println(F("initGSM krok 0"));
  #endif
  sendCommandGSM("AT",2,odp_init[0], rozkazWykonany_init[0]);
  if( rozkazWykonany_init[0] ) {
    #ifdef DEBUG
      Serial.println(F("initGSM krok 1"));
    #endif
    sendCommandGSM("AT+CREG?",1,odp_init[1], rozkazWykonany_init[1]);
    if( rozkazWykonany_init[1] ) {
      #ifdef DEBUG
        Serial.println(F("initGSM krok 2"));
      #endif
      if(odp_init[1] == "+CREG: 0,1\r\n\r\nOK") { // isConnected
        sendCommandGSM("AT+SAPBR=3,1,\"Contype\",\"GPRS\"",1,odp_init[2], rozkazWykonany_init[2]);
        if( rozkazWykonany_init[2] ) {
          if(odp_init[2] == "ERROR") {
             #ifdef DEBUG
              Serial.println(F("initGSM gsm connection ERROR"));
            #endif
            // todo reset sim i od nowa przez <retry> ilosc razy
          }
          else {
            #ifdef DEBUG
              Serial.println(F("initGSM krok 3"));
            #endif
            sendCommandGSM("AT+SAPBR=3,1,\"APN\",\"internet\"",1,odp_init[3], rozkazWykonany_init[3]);
            if( rozkazWykonany_init[3] ) {
              #ifdef DEBUG
                Serial.println(F("initGSM krok 4"));
              #endif
              sendCommandGSM("AT+SAPBR=3,1,\"USER\",\"\"",1,odp_init[4], rozkazWykonany_init[4]);
              if( rozkazWykonany_init[4] ) {
                #ifdef DEBUG
                  Serial.println(F("initGSM krok 5"));
                #endif
                sendCommandGSM("AT+SAPBR=3,1,\"PWD\",\"\"",1,odp_init[5], rozkazWykonany_init[5]);
                if( rozkazWykonany_init[5] ) {
                  #ifdef DEBUG
                    Serial.println(F("initGSM krok 6"));
                  #endif
                  sendCommandGSM("AT+SAPBR=1,1",1,odp_init[6], rozkazWykonany_init[6]);
                  if( rozkazWykonany_init[6] ) {
                    #ifdef DEBUG
                      Serial.println(F("initGSM finish"));
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
      }
    }
  }
  return false;
}

bool rozkazWykonany_post[9] = {false};
String odp_post[9] = {""};

void clearAllFlagsPostGsm() {
  for(uint8_t i = 0; i < 9; i++) {
    rozkazWykonany_post[i] = false;
  }
  batteryPobrane = false;
  rozkazWykonany_battery = false;
  sendingData = false;
}

void makePostGSM() {

  #ifdef DEBUG
    Serial.println(F("postGSM krok 0"));
  #endif
  sendCommandGSM("AT",2,odp_post[0], rozkazWykonany_post[0]);
  if( rozkazWykonany_post[0] ) {
    #ifdef DEBUG
      Serial.println(F("postGSM krok 1"));
    #endif
    sendCommandGSM("AT+HTTPINIT",2,odp_post[1], rozkazWykonany_post[1]);
    if( rozkazWykonany_post[1] ) {
      if( odp_post[1] != "OK" ) {
        // błąd, retry
        #ifdef DEBUG
          Serial.println(F("postGSM krok 1 - błąd, retry"));
        #endif
        // TODO
      }
      else {
        #ifdef DEBUG
          Serial.println(F("postGSM krok 2"));
        #endif
        sendCommandGSM("AT+HTTPPARA=\"URL\",\""+serverAndRes+"\"",2,odp_post[2], rozkazWykonany_post[2]);
        if( rozkazWykonany_post[2] ) {
          #ifdef DEBUG
            Serial.println(F("postGSM krok 3"));
          #endif
          sendCommandGSM("AT+HTTPPARA=\"CONTENT\",\"application/json\"",2,odp_post[3], rozkazWykonany_post[3]);
          if( rozkazWykonany_post[3] ) {
            #ifdef DEBUG
              Serial.println(F("postGSM krok 4"));
            #endif
            sendCommandGSM("AT+HTTPDATA=" + String(dataToSend.length()) + ",100000",2,odp_post[4], rozkazWykonany_post[4]);
            if( rozkazWykonany_post[4] ) {
              #ifdef DEBUG
                Serial.println(F("postGSM krok 5"));
              #endif
              sendCommandGSM(dataToSend,2,odp_post[5], rozkazWykonany_post[5]);
              if( rozkazWykonany_post[5] ) {
                #ifdef DEBUG
                  Serial.println(F("postGSM krok 6"));
                #endif
                sendCommandGSM("AT+HTTPACTION=1",2,odp_post[6], rozkazWykonany_post[6]);
                if( rozkazWykonany_post[6] ) {
                  // todo akcja na odp_post[6] (wyciecie http status)
                  // jesli inny niz 200 to retry? albo zarzucenie (reset sima) i retry
                  #ifdef DEBUG
                    Serial.println(F("postGSM krok 7"));
                  #endif
                  sendCommandGSM("AT+HTTPREAD",2,odp_post[7], rozkazWykonany_post[7]);
                  if( rozkazWykonany_post[7] ) {
                    // todo w tym miejcu mamy response w odp_post7
                    #ifdef DEBUG
                      Serial.println(F("postGSM krok 8"));
                    #endif
                    sendCommandGSM("AT+HTTPTERM",2,odp_post[8], rozkazWykonany_post[8]);
                    
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
