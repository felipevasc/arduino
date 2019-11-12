#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h> 
#include "inicio.h";

const char* ssid = "CAGECE-IOT";
const char* password = "iot12345";
char rede[50];
char senha[50];

WebServer server(80);

String html() {
  String mensagem ="";
          mensagem += "<html style='width: 100%;'><head><meta name='viewport' content='width=device-width, user-scalable=no'></head><body style='background-color: #eeeff2; style='min-width: 100%;'>";
          mensagem +=  "<table style='width: 100%; height: 98%;'>";
          mensagem += "<tr><td style='width: 99%; height: 100%; margin: 0 auto; height: 100%; background-color: #FFFFFF; box-shadow: 0px 0px 90px 0px rgba(184, 184, 184, 1); border-radius: 10px; text-align: center; padding: 0; vertical-align: top;'>";
          mensagem += "<h1 style='font-family: Helvetica, Arial; color: #FFFFFF; background-color: rgb(46, 163, 242); margin: 0; padding: 0; border-top-left-radius: 10px; border-top-right-radius: 10px;'>Cagece</h1><br/><hr/><br/>";
          mensagem += "<h3>Smart Shots<h3>";
          mensagem += "<form method='GET' action='conectar'>";
          mensagem += "[REDES]";
          mensagem += "<br/><input style='border-radius: 10px; height: 40px; padding-left: 10px; width: 90%; margin: 0 auto; font-size: 15px; -webkit-appearance: none; -moz-appearance:textfield;' name='senha' type='password' placeholder='Senha'/><hr/><br/>";
          mensagem += "<input style='border-radius: 10px; height: 40px; padding-left: 10px; width: 50%; margin: 0 auto; font-size: 15px;' class='button' type='submit' value='Entrar'>";
          mensagem += "</form>";
          mensagem += "</td></tr>";
          mensagem += "</table>";
          mensagem += "</body></html>";
          return mensagem;
}
void inicio() {
  String s = PAGINA_INICIO;
  server.send(200, "text/html", s);
}
void ligarDesligarBomba() {
  int slave = server.arg(0).toInt();
  int acao = server.arg(1).toInt();
  String destino = "S"+String(slave);
  if (acao == 0 || acao == 1) {
    StaticJsonDocument<200> json;
    json["acao"] = acao;
    json["evento"] = "ligar";
    json["key"] = key[slave - 1];
    json["origem"] = ID;
    json["destino"] = destino;
    bomba[slave - 1] = acao;
    enviarLora(json);
    server.send(200, "text/html", "OK");
  }
}
void ledON() {
    int pino;
    int i = server.arg(0).toInt();
    pino = i;
    digitalWrite(pino, 1);
    server.send(200, "text/html", "OK");
}
void ledOFF() {
    int pino;
    int i = server.arg(0).toInt();
    pino = i;
    digitalWrite(pino, 0);
    server.send(200, "text/html", "OK");
}
void check() {
  String mensagem = "{";
  if (S1 != "") mensagem += "\"S1\":"+S1+",";
  if (S2 != "") mensagem += "\"S2\":"+S2+",";
  if (S3 != "") mensagem += "\"S3\":"+S3+",";
  if (S4 != "") mensagem += "\"S4\":"+S4+",";
  if (S5 != "") mensagem += "\"S5\":"+S5+",";
  if (S6 != "") mensagem += "\"S6\":"+S6+",";
  long tempo = millis();
  mensagem += "\"tempo\":"+String(tempo)+"}";
  server.send(200, "text/html", mensagem);
}

void configurar() {
  String message = "";
  String select = "";
  Serial.print("scannetworks:");
  int n = WiFi.scanNetworks();
  select += "<div style='width: 90%; height: 150px; margin: 0 auto; padding-left: 10px; border-radius: 10px; overflow-y: scroll;'>";
  String cor;
  Serial.print(n);
  for (int i = 0; i < n; i++) {
    if (i % 2 == 0) {
      cor = "rgba(156, 233, 252, 0.2)";
    }
    else {
      cor = "rgba(156, 233, 252, 0.1)";
    }
    select += "<label style='display: inline-block; width: 98%; margin: 0 auto; font-size: 16px; padding: 4px; text-align: left; background-color: "+cor+"; border-radius: 4px; margin: 2px;'><input name='rede' type='radio' value='"+String(WiFi.SSID(i))+"'> " + String(WiFi.SSID(i)) + " (" + String(WiFi.RSSI(i)) + "%)</label>";  
  }
  select += "</div>";
  
  message += html();
  message.replace("[REDES]", select);
  server.send(200, "text/html", message);
}
void conectar() { 
  server.arg(0).toCharArray(rede, 50);
  server.arg(1).toCharArray(senha, 50);
  Serial.print("SSID: " + String(ssid));
  Serial.println("password: " + String(password));
  WiFi.begin(ssid, password);
  int y = 0;
  while (WiFi.status() != WL_CONNECTED && y < 10) {
    delay(1000);
    Serial.printf("Status da Conexao: %d\n", WiFi.status());
    y++;
  }
  
  Serial.printf("Conexao atual (apos tentar conectar): %d\n", WiFi.status());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("APIP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("LOCALIP: ");
  Serial.println(WiFi.localIP());
  Serial.print("waitForConnectResult: ");
  Serial.println(WiFi.waitForConnectResult());

  String message = "";
  if (WiFi.SSID() == "0.0.0.0") {
    message += "Falha ao tentar se conectar.";
  }
  else {
    message += "Conectado na rede, IP assumido: "+WiFi.localIP();
  }
  server.send(200, "text/html", message);
}

void conectarWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.persistent(true);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/ligarDesligarBomba", ligarDesligarBomba);
  server.on("/configurar", configurar);
  server.on("/conectar", conectar);
  server.on("/ledON", ledON); 
  server.on("/ledOFF", ledOFF);
  server.on("/check", check);
  server.on("/", inicio);
  server.begin();
}

void configurarWifi() {
  int numberOfNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numberOfNetworks; i++) {
 
    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));
 
    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));
 
    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));
 
    Serial.print("Encryption type: ");
    String encryptionTypeDescription = String(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
 
    Serial.println("-----------------------");
 
  }
}

String translateEncryptionType(int encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}
