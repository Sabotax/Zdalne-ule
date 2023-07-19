void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
}

void loop() {
  Serial.println(String(touchRead(0))+"\t"+String(touchRead(4))+"\t"+String(touchRead(32))+"\t"+String(touchRead(33)));  // get value of Touch 0 pin = GPIO 4
  delay(1000);
}
// na 80
