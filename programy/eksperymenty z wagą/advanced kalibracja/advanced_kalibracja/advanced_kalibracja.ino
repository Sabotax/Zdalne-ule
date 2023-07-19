#include <HX711.h>

#define DOUT  26         //pin 3 Arduino i wyjście DAT czujnika
#define CLK  25           //pin 2 Arduino i wyjście CLK czujnika

HX711 loadcell;
 
void setup() {
  setCpuFrequencyMhz(80);
  Serial.begin(115200);
  Serial.println("HX711 - program kalibracyjny");


  loadcell.begin(DOUT, CLK);
  loadcell.set_scale();
  loadcell.set_offset();
 
}

char c;
String buf = "";
byte rozkaz=0;
float offset = 0;
float scale = 0;
bool measure = false;

void loop() {

  while(Serial.available()) {
    c = Serial.read();
    if(c == ';') {
      if(buf == "O") {
        rozkaz = 1;
      }
      if(buf == "S") {
        rozkaz = 2;
      }
      buf = "";
    }
    else if(c =='\n') {
      if(rozkaz == 1) {
        offset = buf.toFloat();
        loadcell.set_offset(offset);
        Serial.println("Offset set to " + String(offset));
      }
      if(rozkaz == 2) {
        scale = buf.toFloat();
        loadcell.set_scale(scale);
        Serial.println("Scale set to " + String(scale));
      }
      rozkaz = 0;
      buf = "";
    }
    else if(c == 'P') {
      rozkaz = 0;
      buf = "";

      measure = true;
    }
    else {
      buf = buf + String(c);
    }
  }

  if(measure) {
    float pierwszy_pomiar = loadcell.get_units();
    float sum = pierwszy_pomiar;
    float min = pierwszy_pomiar;
    float max = pierwszy_pomiar;
    
    for(byte i = 1 ; i < 20; i++ ) {
      float pomiar = loadcell.get_units();
      Serial.println("Pomiar " + String(i) + ": " + String(pomiar));
      sum += pomiar;
      if(pomiar < min) min = pomiar;
      if(pomiar > max) max = pomiar;
      
      delay(50);
    }
    sum = sum / 20;

    Serial.println("Srednia: " + String(sum));
    Serial.println("Min: " + String(min));
    Serial.println("Max: " + String(max));
  }
}
