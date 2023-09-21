// WAGA
#include <HX711.h>
#define DOUT  26
#define CLK  25 
HX711 loadcell;

float wagaOdczyt;

RTC_DATA_ATTR float weight_offset = 970500;
RTC_DATA_ATTR float weight_scale = 41990;

void initMyWaga() {
  //WAGA
  loadcell.begin(DOUT, CLK);
  loadcell.set_scale(weight_scale);
  loadcell.set_offset(weight_offset);
}
