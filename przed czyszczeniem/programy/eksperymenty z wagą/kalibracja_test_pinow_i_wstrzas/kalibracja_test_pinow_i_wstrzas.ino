#include <HX711.h>
 
#define DOUT  35         
#define CLK  34           
#define wstrzasD 35
#define wstrzasA 34
 
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

  pinMode(wstrzasD,INPUT);
  pinMode(wstrzasA,INPUT);
 
}

void loop() {
 
//  scale.set_scale(calibration_factor);       //Wyrównanie według współczynnika kalibracji
//todo budzenie się na pinach wstrząsowych
 
  Serial.print("Waga:\t");
  Serial.print(loadcell.get_units(7));
  Serial.print("\tWstrzasA\t");
  Serial.print(analogRead(wstrzasA));
  Serial.print("\tWstrzasD\t");
  Serial.print(digitalRead(wstrzasD));
 Serial.println();
                                                                                                                                                                                                                                                                                 
  delay(50);

}
