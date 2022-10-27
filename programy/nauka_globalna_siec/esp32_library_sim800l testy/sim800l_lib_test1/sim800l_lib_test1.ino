#include <Http.h>
#include <Sim800.h>

//unsigned int RX_PIN = 7;
//unsigned int TX_PIN = 8;
//unsigned int RST_PIN = 12;

unsigned int RX_PIN = 16;
unsigned int TX_PIN = 17;

HTTP http(2, 9600); //, RX_PIN, TX_PIN, RST_PIN

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("start");
  delay(5000);
  Serial.println(String(http.readVoltage()));
}

void loop() {
  // put your main code here, to run repeatedly:

}
