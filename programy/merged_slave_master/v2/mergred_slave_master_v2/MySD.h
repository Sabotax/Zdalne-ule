//SD
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//char bufor[32768];
//uint16_t bufor_index = 0;
File file;

void saveDataToSD(fs::FS &fs,const String& data) {
  String filename = "/"+getDay()+".csv";
  Serial.println("filename: "+filename);
  file = fs.open(filename, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    //reportBug(29);
    return;
  }
  else {
    if(file.print(data)){
      Serial.println("Message appended - saveDataToSD");
    } else {
      Serial.println("Append failed");
      //reportBug(30);
    }
    file.close();
  }
}

void initMySD() {
  //SD
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      //reportBug(27);
      return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      //reportBug(28);
      return;
  }
}
