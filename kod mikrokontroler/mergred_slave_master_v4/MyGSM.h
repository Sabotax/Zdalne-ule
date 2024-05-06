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
  }

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
      responseMsg = response;
      rozkazWykonany = gotResponse;
    }
    return gotResponse;
  }
}


// progress

// reset Sim

// isConnected

// get Battery

// get Signal

// get Http status

// handshake

bool rozkazWykonany_init[7] = {false};
String odp_init[7] = {""};

void initGSM() {
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
}

bool rozkazWykonany1 = false; // jesli juz raz go zrobi, to zeby w petli od razu przeszlo do kolejnego

//void makePostGSM(String body, bool retry) {
//
//  String odp1 = "";
//  sendCommandGSM("AT",3,odp1, rozkazWykonany1)
//  if( rozkazWykonany1 ) { // todo na koniec sekwencji reset flag
//    // todo zagniezdzone kolejne kroki
//  }
//
//}
