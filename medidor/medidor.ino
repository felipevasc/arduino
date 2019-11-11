#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

 
const char* ssid = "CAGECE-DIR";
const char* password = "senha";
 
void setup () {
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.begin(115200);
  WiFi.begin(ssid);
  //WiFi.begin(ssid, password);
  int x = 0;
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED && x < 20) {
    delay(1000);
    Serial.print(".");
    x++;
  }
  Serial.println("softAPIP:" + WiFi.softAPIP().toString());
  Serial.println("SSID:" + WiFi.SSID());
  Serial.println("localIP:" + WiFi.localIP().toString());
  Serial.println("MAC:" + WiFi.macAddress());
  pinMode(D1, INPUT);
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    //if (true) {
      
    if (digitalRead(D1) < 1) {
      
      Serial.print("Chamar http:");
      
      HTTPClient http; //Object of class HTTPClient
      int beginResult = http.begin("http://172.30.93.32/infra/iot_medidor/add.php");
      //int beginResult = http.begin("https://sistemas.int.cagece.com.br/backup/iot_medidor/add.php", "f4ac6baec67267c1c9009cc8a9c05a4bb606a5e3");
      http.addHeader("Content-Type", "text/plain");
      int httpCode = http.GET();
      String response = http.getString();
      
      Serial.print("beginResult: ");
      Serial.print(beginResult);
      Serial.println();
      
      Serial.print("http: ");
      Serial.print(httpCode);
      Serial.println();
      
      Serial.print("response: ");
      Serial.println(response);
      Serial.println();

      http.end();
      delay(3000);
      
    } 
      
  }
}
