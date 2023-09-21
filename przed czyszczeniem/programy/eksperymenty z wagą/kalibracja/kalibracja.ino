#include <HX711.h>
 
//#define DOUT  16         //pin 3 Arduino i wyjście DAT czujnika
//#define CLK  4           //pin 2 Arduino i wyjście CLK czujnika

#define DOUT  26         //pin 3 Arduino i wyjście DAT czujnika
#define CLK  25           //pin 2 Arduino i wyjście CLK czujnika
//#define DOUT 3
//#define CLK 2
//#define wstrzas 32
//d4 data
//d2 clk
//d5 wstzra do
 
HX711 loadcell;
 
void setup() {
  setCpuFrequencyMhz(80);
  Serial.begin(115200);
  Serial.println("HX711 - program kalibracyjny");
  Serial.println("Usun obciazenie z belki");
  Serial.println("Po rozpoczeciu odczytu, umiesc znana mase na belce");
  Serial.println("Wcisnij + lub a by zwiekszyc wspolczynnik kalibracji");
  Serial.println("Wcisnij - lub z by zwiekszyc wspolczynnik kalibracji");

  loadcell.begin(DOUT, CLK);
  loadcell.set_scale();
  loadcell.set_offset();
 
}

void loop() {
 
//  scale.set_scale(calibration_factor);       //Wyrównanie według współczynnika kalibracji
 
  Serial.print("Odczyt: ");
  Serial.println(loadcell.get_units(2));
 
                                                                                                                                                                                                                                                                                 
  delay(50);

}
