const char* espMasterId = "esp01";

String SlaveDataShotToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp) {
  String re="[{\"S\": \""; //espSlaveId
  re += espSlaveId;

  re += "\"},{\"W\": \""; //waga
  re += String(waga) + "\"}";
  //timestamp
  re += ",{\"I\": \"";
  //temp wewn
  re += String(temperatura) + "\"}]";

  return re;
}

String WholeDataToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp,const float& temperatura_zewn) {
  String re = "";
  re += "{";
  re += "\"T\": \"Watykanczyk2137\","; //esp master id
  re += "\"M\": \"" + String(espMasterId) + "\",";
  re += "\"C\": \"" + String(temperatura_zewn) + "\","; 
  re += "\"A\": \"" + myTimestamp + "\","; //esp master id
  re += "\"D\": " + SlaveDataShotToJson(espSlaveId,waga,temperatura); //dane
  re += "}";
  return re;
}

void reportBug(uint16_t code) {
  #ifdef DEBUG
    Serial.print("Zgłoszono bug o kodzie "+String(code));
    Serial.flush();
    // 10-Couldn't find RTC
  #endif
}
