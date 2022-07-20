#define RXD2 16
#define TXD2 17
void setup() {
  Serial.begin(115200);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  delay(3000);
  Serial.print("Start - wysyłam AT");
  Serial2.print("AT");
}
void loop() {
  updateSerial(); 
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
