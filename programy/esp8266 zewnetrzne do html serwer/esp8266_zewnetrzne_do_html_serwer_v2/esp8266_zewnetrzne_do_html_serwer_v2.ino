/*
  Arduino Webserver using ESP8266
  Displays temperature in a webpage

  Arduino Mega has three Serial communication ports,this code works well with 
  Arduino Mega.For UNO users,use Softserial library to connect ESP8266 with
  Arduino UNO 
  If you're unsure about your Arduino model or its pin cofigurations,please check
  the documentation at http://www.arduino.cc
 
  modified August 2016
  By Joyson Chinta and Gladson Kujur
 */
#include <SoftwareSerial.h>
SoftwareSerial esp(3,2); // RX TX
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);    ///////For Serial monitor 
  Serial.println("Start pracy");
  esp.begin(115200); ///////ESP Baud rate
  //pinMode(11,OUTPUT);    /////used if connecting a LED to pin 11
  //digitalWrite(11,LOW);
 
  sendData("AT+RST\r\n",3000,DEBUG); // reset module
  //wlasne start
  sendData("AT+GMR\r\n",4000,DEBUG);
  //wlasne end
  sendData("AT+CWMODE=2\r\n",4000,DEBUG); // configure as access point
  //własne start
  String ssid = "esp8266_test";
  String password = "12345678";
  String RF_channel = "11";
  String security_scheme = "3"; //WPA2_PSK
  sendData("AT+CWSAP=\""+ssid+"\",\""+password+"\","+RF_channel+","+security_scheme+"\r\n",3000,DEBUG); //AT+CWSAP="esp8266_test","12345678",11,3
  //własne end
  sendData("AT+CIFSR\r\n",2000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",2000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",2000,DEBUG); // turn on server on port 80
}


float sensetemp() ///////function to sense temperature.
 {
//  int val = analogRead(A0);
//  float mv = ( val/1024.0)*5000; 
//  float celcius = mv/10;
//  return(celcius);
return (int) millis()/1000;
 }
 
int connectionId;
void loop()
{
  if(esp.available())
  {
    /////////////////////Recieving from web browser to toggle led
    if(esp.find("+IPD,"))
    {
     delay(300);
     connectionId = esp.read()-48;
     if(esp.find("pin="))
     { 
     Serial.println("recieving data from web browser");
     int pinNumber = (esp.read()-48)*10; 
     pinNumber += (esp.read()-48); 
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
 
  //////////////////////////////sends data from ESP to webpage///////////////////////////
 
 void espsend(String d)
         {
             String cipSend = " AT+CIPSEND=";
             cipSend += connectionId; 
             cipSend += ",";
             cipSend +=d.length();
             cipSend +="\r\n";
             sendData(cipSend,1000,DEBUG);
             sendData(d,1000,DEBUG); 
         }

//////////////gets the data from esp and displays in serial monitor///////////////////////         
String sendData(String command, const int timeout, boolean debug)
            {
                String response = "";
                esp.print(command);
                long int time = millis();
                while( (time+timeout) > millis() )
                {
                   while(esp.available())
                      {
                         char c = esp.read(); // read the next character.
                         response+=c;
                      }  
                }
                
                
                if(debug)
                     {
                      if(response =="") {
                        Serial.println("command:"+command+" has timeout");
                      }
                      else {
                        Serial.println(response); //displays the esp response messages in arduino Serial monitor
                      }
                      
                     }
                return response;
            }
