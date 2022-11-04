const char* espMasterId = "esp01";

String SlaveDataShotToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp,const float& temperatura_zewn) {
  String re="[{\"S\": \""; //espSlaveId
  re += espSlaveId;

  re += "\"},{\"W\": \""; //waga
  re += String(waga) + "\"},{\"T\": \"";
  //timestamp
  re += myTimestamp + "\"},{\"I\": \"";
  //temp wewn
  re += String(temperatura) + "\"},{\"O\": \"";
  //temp out
  re += String(temperatura_zewn)+"\"}]";

  return re;
}

String WholeDataToJson(const String& espSlaveId, const float& waga ,const float& temperatura, const String& myTimestamp,const float& temperatura_zewn) {
  String re = "";
  re += "{";
  re += "\"T\": \"Watykanczyk2137\","; //esp master id
  re += "\"M\": \"" + String(espMasterId) + "\","; //esp master id
  re += "\"D\": " + SlaveDataShotToJson(espSlaveId,waga,temperatura,myTimestamp,temperatura_zewn); //dane
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
