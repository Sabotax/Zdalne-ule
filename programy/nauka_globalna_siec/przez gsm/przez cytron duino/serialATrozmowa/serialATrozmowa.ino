#include <SoftwareSerial.h>
SoftwareSerial SerialSIM(4,5); //rx,tx
void setup() {
  Serial.begin(9600);
  SerialSIM.begin(9600);
  Serial.println("START");
}

void loop() {
  if (SerialSIM.available()) {
    Serial.write(SerialSIM.read());
  }
  delay(5);
  if (Serial.available()) {
    SerialSIM.write(Serial.read());
  }
  delay(5);
}
