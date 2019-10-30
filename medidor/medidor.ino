#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "Brisa";
const char* password = "seriaprasempre";
 
void setup () {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (digitalRead(D1) < 1) {
      HTTPClient http;
      http.begin("https://sistemas.int.cagece.com.br/backup/iot_medidor/add.php");
      int httpCode = http.GET();
      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
      }
      http.end();   
      delay(300);
    }
      
  }
  
}
