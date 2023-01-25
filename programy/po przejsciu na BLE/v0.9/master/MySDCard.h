/*
 * Założenia:
 * -zapisuje dane na kartę na plik tekstowy (każdego dnia jest inny plik tekstowy), a poszczególne krotki dodaje w linii
 * -telefon będzie mógł zdobyć te dane i przechować (nie wiadomo jeszcze czy używając BLE czy włączanego na chwilę wifi)
 * -następnie telefon wyśle dane i przechowa odpowiedź serwera (który wkładając sprawdza, żeby się nie powtarzały krotki itp) mówiącą od którego dnia i którego esp ma dane
 * -następnie przy kolejnym łączeniu z tym konkretnym esp, esp dostanie dane do którego dnia może usunąć dane (bo znajdują się już na serwerze)
 * 
 * odnośnie przesyłania danych na telefon:
 * można skorzystać z BLE, (max 512 bajtów na usługę, czyli 512 znaków) ale trzeba w takim razie zrobić coś w stylu: przesyła około 500 znaków, kończy przesyłanie ich gdzieś 
 * (może w drugiej usłudze) informuje że dalej trzeba i że jeszce nie skończyło i dalej leci przesyłanie i tak w kółko, dzięki temu "protokołowi przesyłania większych ilości danych
 * over BLE" nie będzie problemów z włączaniem wifi i hotspot itp, więc tak będzie chyba lepiej, prądowo też
 * https://github.com/petewarden/ble_file_transfer
 * 
 * fun fact, dzięki wystawieniu informacji na początku jak duży jest plik/jego zawartość będzie można zrobić progress bar zwiększający się co iteracje tego protokołu
 * 
 * edit: jednak łatwiej chyba będzie zrobić charakterystykę na BLE która włącza wifi i wtedy po prostu przesłać POSTem bo inaczej to będzie udręka
 */

#include "FS.h"
#include "SD.h"
#include "SPI.h"

char bufor[32768];
uint16_t bufor_index = 0;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
        reportBug(20);
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
        reportBug(21);
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        reportBug(22);
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
        reportBug(23);
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
        reportBug(24);
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
        reportBug(26);
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
        reportBug(25);
    }
}

void initSD() {
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        reportBug(27);
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        reportBug(28);
        return;
    }
}


void saveDataToSD(fs::FS &fs,const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp) {
  String filename = "/"+getDay()+".json";
  //Serial.println("filename: "+filename);
  File file = fs.open(filename, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    reportBug(29);
    return;
  }
  else {
    String message = SlaveDataShotToJson(espSlaveId,waga,temperatura,myTimestamp)+";";
    if(file.print(message)){
      Serial.println("Message appended - saveDataToSD");
    } else {
      Serial.println("Append failed");
      reportBug(30);
    }
    file.close();
  }
}
