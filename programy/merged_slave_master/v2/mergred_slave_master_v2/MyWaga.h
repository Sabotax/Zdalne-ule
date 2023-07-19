// WAGA
#include <HX711.h>
#define DOUT  26
#define CLK  25 
HX711 loadcell;

float wagaOdczyt;

void initMyWaga() {
  //WAGA
  loadcell.begin(DOUT, CLK);
  loadcell.set_scale();
  loadcell.set_offset();
}
