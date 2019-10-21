//Compila apenas se MASTER estiver definido no arquivo principal
#ifdef MASTER

//Intervalo entre os envios
#define INTERVAL 1000

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h> 
#include "inicio.h"

const char* ssid = "Brisa";
const char* password = "serieaprasempre";
char rede[50];
char senha[50];


//Tempo do último envio
long lastSendTime = 0;
WebServer server(80);

String header;
void setup(){
  Serial.begin(115200);
  //Chama a configuração inicial do display
  setupDisplay();
  //Chama a configuração inicial do LoRa
  setupLoRa();
  configurarWifi();
  pinMode(LED, OUTPUT);
  pinMode(LIGAR, INPUT);
  mostrarTexto("Master ", "Aguardando conexao");
  mostrar();
  
  //configurarWifi();
  //conectar();
  WiFi.mode(WIFI_STA);
  WiFi.persistent(true);
  //WiFi.softAP("ControleCagece", "cagece");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  //Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.macAddress());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/configurar", configurar);
  server.on("/conectar", conectar);
  server.on("/ledON", ledON); 
  server.on("/ledOFF", ledOFF);
  server.on("/check", check);
  server.on("/", inicio);
  server.begin();
}

void loop(){
  //Se passou o tempo definido em INTERVAL desde o último envio
  if (millis() - lastSendTime > INTERVAL){
    //Marcamos o tempo que ocorreu o último envio
    lastSendTime = millis();
    //Envia o pacote para informar ao Slave que queremos receber os dados
    send();
  }

  //Verificamos se há pacotes para recebermos
  receive();
  mostrar();

  server.handleClient();
}

void send(){
  //Inicializa o pacote
  LoRa.beginPacket();

  led = digitalRead(LED);
  ligar = digitalRead(LIGAR);
  String mensagem = "{ligar:"+String(ligar)+",led:"+led+"}"; 
  LoRa.print(mensagem);
  
  //Finaliza e envia o pacote
  LoRa.endPacket();
  mostrarTexto1("S:" + mensagem);
}

void receive(){
  //Tentamos ler o pacote
  int packetSize = LoRa.parsePacket();
  mostrarTexto3(String(LoRa.packetRssi()));
  //Verificamos se o pacote tem o tamanho mínimo de caracteres que esperamos
  if (packetSize > 2){
    String received = "";
    //Armazena os dados do pacote em uma string
    while(LoRa.available()){
      received += (char) LoRa.read();
    }
    String waiting = String(millis() - lastSendTime);
    int index = received.indexOf("alto:");
    if (index >= 0) {
      alto = received.substring(index + 5, index + 6).toInt();
    }
    index = received.indexOf("baixo:");
    if (index >= 0) {
      baixo = received.substring(index + 6, index + 7).toInt();
    }
    
    mostrarTexto2("R:"+received);
  }
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

void inicio() {
  String s = PAGINA_INICIO;
  server.send(200, "text/html", s);
}

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
  String mensagem = "{\"led\":"+String(led)+",\"ligar\":"+ligar+",\"alto\":"+alto+",\"baixo\":"+baixo+"}";
  server.send(200, "text/html", mensagem);
}

#endif
