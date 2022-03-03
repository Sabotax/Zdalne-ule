#include <SoftwareSerial.h>
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

SoftwareSerial wifiSerial(2, 3);      // RX, TX for ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout
int connectionId;
/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

void espsend(String d) {
  String cipSend = " AT+CIPSEND=";
  cipSend += connectionId; 
  cipSend += ",";
  cipSend +=d.length();
  cipSend +="\r\n";
  sendData(cipSend,1000,DEBUG);
  sendData(d,1000,DEBUG); 
}

// symulacja jakiegoś pomiaru
float sensetemp() {
//  int val = analogRead(A0);
//  float mv = ( val/1024.0)*5000; 
//  float celcius = mv/10;
//  return(celcius);

  return 47;
}


void setup() {
  pinMode(13,OUTPUT);  //set build in led as output
  Serial.begin(115200);
  wifiSerial.begin(115200);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}

void loop() {
  if(wifiSerial.available())
  {
    /////////////////////Recieving from web browser to toggle led
    if(wifiSerial.find("+IPD,"))
    {
     delay(300);
     connectionId = wifiSerial.read()-48;
     if(wifiSerial.find("pin="))
     { 
     Serial.println("recieving data from web browser");
     int pinNumber = (wifiSerial.read()-48)*10; 
     pinNumber += (wifiSerial.read()-48); 
     digitalWrite(pinNumber, !digitalRead(pinNumber));
     }
   
    /////////////////////Sending data to browser
    else
    {
      String webpage = "<h1>Hello World</h1>";
      espsend(webpage);
     }
    
     if(sensetemp() != 0)
     {
       String add1="<h4>Temperature=</h4>";
      String two =  String(sensetemp(), 3);
      add1+= two;
      add1+="&#x2103";   //////////Hex code for degree celcius
      espsend(add1);
     }
    
     else
     {
      String c="sensor is not conneted";
      espsend(c);                                     
     } 
     
     String closeCommand = "AT+CIPCLOSE=";  ////////////////close the socket connection////esp command 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     sendData(closeCommand,3000,DEBUG);
    }
  }
}
