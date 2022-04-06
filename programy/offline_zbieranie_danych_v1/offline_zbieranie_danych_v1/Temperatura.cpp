#include "Temperatura.h"

boolean Temperatura::init() {
      // zwraca true, jesli sie udalo (uzyc jako warunek w if)
      if ( !ds.search(addr) ) {
        if (DEBUG) Serial.println("Sonda temperatury - nie znaleziono adresow");
        ds.reset_search();
        delay(250);
        return false;
      } else {
        if (DEBUG) Serial.print("Znaleziono sondę o adresie =" );
        for(byte el: addr) {
          if (DEBUG) Serial.write(' ');
          if (DEBUG) Serial.print(el,HEX);
        }

        if (OneWire::crc8(addr, 7) != addr[7]) {
          if (DEBUG) Serial.println("CRC is not valid!");
          return false;
        }

        switch (addr[0]) {
          case 0x10:
            if (DEBUG) Serial.println("  Chip = DS18S20");  // or old DS1820
            type_s = 1;
            break;
          case 0x28:
            if (DEBUG) Serial.println("  Chip = DS18B20");
            type_s = 0;
            break;
          case 0x22:
            if (DEBUG) Serial.println("  Chip = DS1822");
            type_s = 0;
            break;
          default:
            if (DEBUG) Serial.println("Device is not a DS18x20 family device.");
            return false;
        } 
      }

      ds.reset();
      ds.select(addr);
      ds.write(0x44,1);
      delay(1000);
      return true;
}
float Temperatura::measure() {
        byte present = ds.reset();
      ds.select(addr);    
      ds.write(0xBE);
      
      if (DEBUG) Serial.print("  Data = ");
      if (DEBUG) Serial.print(present, HEX);
      if (DEBUG) Serial.print(" ");
      for (byte i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
        if (DEBUG) Serial.print(data[i], HEX);
        if (DEBUG) Serial.print(" ");
      }
      if (DEBUG) Serial.print(" CRC=");
      if (DEBUG) Serial.print(OneWire::crc8(data, 8), HEX);
      if (DEBUG) Serial.println();

      int16_t raw = (data[1] << 8) | data[0];
      if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (type_s) {
          raw = raw << 3; // 9 bit resolution default
          if (data[7] == 0x10) {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
          }
        } else {
          byte cfg = (data[4] & 0x60);
          // at lower res, the low bits are undefined, so let's zero them
          if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
          else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
          else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
          //// default is 12 bit resolution, 750 ms conversion time
        }
      }
    float celsius = (float)raw / 16.0;
    return celsius;
}
