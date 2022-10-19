#define RX1 9
#define TX1 10

#define RX2 16
#define TX2 17
//#include <HardwareSerial.h>

void updateSerial(String msg)
{
  Serial2.println(msg);
  delay(100);
  while (Serial.available()) 
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial2.available()) 
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  Serial.println("---Start---");

  updateSerial("AT");//Once the handshake test is successful, it will back to OK
  updateSerial("AT+CSQ");//Signal quality test, value range is 0-31 , 31 is the best
  updateSerial("AT+CCID");//Read SIM information to confirm whether the SIM is plugged
  updateSerial("AT+CREG?");//Check whether it has registered in the network
  updateSerial("AT+CMGF=1");
  updateSerial("AT+CMGS=\"+48537411356\"");
  updateSerial("witam, pozdrawiam z esp");
  Serial2.write(26);
}

void loop() {
}
