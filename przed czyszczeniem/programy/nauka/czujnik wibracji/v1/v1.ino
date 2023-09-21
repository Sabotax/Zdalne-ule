#define digital_wstrzas 26
#define analog_wstrzas 25

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(digital_wstrzas,INPUT);
  pinMode(analog_wstrzas,INPUT);
}

void loop() {
  Serial.println(String(analogRead(analog_wstrzas))+"\t"+String(digitalRead(digital_wstrzas)));
  delay(10);
}
