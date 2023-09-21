#include <Http.h>
#include <Sim800.h>

//unsigned int RX_PIN = 7;
//unsigned int TX_PIN = 8;
//unsigned int RST_PIN = 12;

unsigned int RX_PIN = 16;
unsigned int TX_PIN = 17;

HTTP http(2, 9600); //, RX_PIN, TX_PIN, RST_PIN

char buffer_gps[80];
char buffer_battery[64];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("start"));
  delay(5000);
  
  Serial.println(F("readVoltage:"));
  Serial.println(String(http.readVoltage()));
  
//  Serial.println(F("read GPS"));
//  http.readGpsLocation(buffer_gps);
//  Serial.println(buffer_gps);

//  Serial.println(F("readVoltagePercentage"));
//  http.readVoltagePercentage(buffer_battery);
//  Serial.println(buffer_battery);
//
//  Serial.println(F("Usypiam SIM (tryb snu 2) i czekam 10 sekund"));
//  http.sleep(false);
//  delay(10000);
//  Serial.println(F("Budze SIM"));
//  http.wakeUp();
//  delay(3000);

  Serial.println(F("Wysyłam example json post"));
  //http.configureBearer("your.mobile.service.provider.apn");
  http.connect();

  char response[256];
  // wlaczanie ssl poprzez wrzucenie do linka https:// (samo wykrywa i ustawia biblioteka)
  Result result = http.post("daniel.rozycki.student.put.poznan.pl/incomingData.php", "{\"date\":\"12345678\"}", response);
  Serial.println(F("Odpowiedz"));
  Serial.println(String(response));

  http.disconnect();
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
