#include "Save.h"

boolean Save::init() {
  if (DEBUG) Serial.print("Initializing SD card...");
  if( !SD.begin(pin_CS) ) {
    if (DEBUG) Serial.println("initialization failed!");
    return false;
  }
  else {
    if (DEBUG) Serial.println("initialization done.");
    initiated_correctly = true;
    return true;
  }
}

boolean Save::save(String nazwa, String text) {
  if (SD.exists(nazwa)) {
    if (DEBUG) Serial.println(nazwa + " exists.");
  } else {
    if (DEBUG) Serial.println(nazwa + " doesn't exist.");
  }

  if (DEBUG) Serial.println("Creating "+nazwa);
  myFile = SD.open(nazwa, FILE_WRITE);

  if (myFile) {
    if (DEBUG) Serial.print("Writing to " + nazwa);
    myFile.println(text);
    // close the file:
    myFile.close();
    if (DEBUG) Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    if (DEBUG) Serial.println("error opening " + nazwa);
  }

}
