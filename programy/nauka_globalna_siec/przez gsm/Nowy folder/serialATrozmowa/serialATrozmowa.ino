#define RX1 9
#define TX1 10

#define RX2 16
#define TX2 17
//#include <HardwareSerial.h>
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  Serial.println("---Start---");
}

void loop() {
  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  delay(5);
  if (Serial.available()) {
    Serial.print("Wysyłam");
    Serial2.write(Serial.read());
  }
  delay(5);


}
