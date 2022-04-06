void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(7,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if( digitalRead(7) == HIGH) {
    digitalWrite(13,LOW);
    
  }
  else {
    digitalWrite(13,HIGH);
    Serial.println(digitalRead(7) );
  }

  delay(10);

}
