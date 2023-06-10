// WAGA
#include <HX711.h>
#define DOUT  32
#define CLK  33 
HX711 loadcell;

void initMyWaga() {
  //WAGA
  loadcell.begin(DOUT, CLK);
  loadcell.set_scale();
  loadcell.set_offset();
}
