#include <DNSServer.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <dht.h>

#include "inicio.h";
#include "funcoesWifi.h";


ESP8266WebServer server(80);

double inicial1;
double inicial2;

void setup() {
  Serial.begin(115200);
  pinMode(D1, INPUT); //SENSOR CORRENTE 1
  pinMode(D2, INPUT); //RELE 1
  pinMode(D3, INPUT); //SENSOR CORRENTE 2
  digitalWrite(D5, 0);
  pinMode(D5, INPUT);
  pinMode(D8, OUTPUT); //RELE 2
  pinMode(D7, OUTPUT); //DHT
  pinMode(A0, INPUT); //MEDIDOR DE CORRENTE

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("SMARTSHOT");

  server.begin();
  Serial.println("Servidor inicializado");
  server.on("/", inicio);
  server.on("/configurar", configurar);
  server.on("/conectar", conectar);
  server.on("/check", check);
  server.on("/ligar1", ligar1);
  server.on("/ligar2", ligar2);
  server.on("/desligar1", desligar1);
  server.on("/desligar2", desligar2);

}

void loop() {
  int x = digitalRead(D1);
  if (x > 0) {
    Serial.print("D1");
    Serial.println(x);
  }
  x = digitalRead(D2);
  if (x > 0) {
    Serial.print("D2");
    Serial.println(x);
  }
  x = digitalRead(D3);
  if (x > 0) {
    Serial.print("D3");
    Serial.println(x);
  }

  server.handleClient();
}

void inicio() {
  String message = PAGINA_INICIO;
  message.replace("[CONTEUDO]", resumo());
  server.send(200, "text/html", message);
}

void configurar() {
  String message = PAGINA_INICIO;
  String conexoes = getConexoesWifi();
  message.replace("[CONTEUDO]", conexoes);
  server.send(200, "text/html", message);
}

void conectar() {
  String message = PAGINA_INICIO;

  char ssid[50];
  char password[50];
  server.arg(0).toCharArray(ssid, 50);
  server.arg(1).toCharArray(password, 50);
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.persistent(true);
  WiFi.begin(ssid, password);
  int y = 0;
  while (WiFi.status() != WL_CONNECTED && y < 20) {
    delay(1000);
    y++;
  }

  message.replace("[CONTEUDO]", resumo());
  server.send(200, "text/html", message);
}

String resumo() {
  String info = "<div style='font-size: 40px;'>";
  info += "<strong>WIFI-softAP:</strong>SMARTSHOT<hr/>";
  info += "<strong>softAP-IP:</strong>" + WiFi.softAPIP().toString() + "<hr/>";
  info += "<strong>WIFI-STA:</strong>" + WiFi.SSID() + "<hr/>";
  info += "<strong>STA-IP:</strong>" + WiFi.localIP().toString() + "<hr/>";
  info += "</div>";
  info += "<div>";
  info += "<input type='button' value='LIGAR 1' onclick=\"window.location.href='/ligar1'\" />";
  info += "<input type='button' value='DESLIGAR 1' onclick=\"window.location.href='/desligar1'\" />";
  info += "<input type='button' value='LIGAR 2' onclick=\"window.location.href='/ligar2'\" />";
  info += "<input type='button' value='DESLIGAR 2' onclick=\"window.location.href='/desligar2'\" />";
  info += "<input type='button' value='CONFIGURAR' onclick=\"window.location.href='/configurar'\" />";
  info += "</div>";
  return info;
}

void check() {
  String info = "{";

  info += "\"R1\":";
  info += String(digitalRead(D2));
  info += ",";
  Serial.println(info);
  info += "\"R2\":";
  info += String(digitalRead(D8));
  info += ",";
  Serial.println(info);
  dht DHT;
#define DHT11_PIN D7
  int chk = DHT.read11(DHT11_PIN);
  if (chk == DHTLIB_OK) {
    info += "\"U\":";
    info += String(DHT.humidity);
    info += ",";
    Serial.println(info);
    info += "\"T\":";
    info += String(DHT.temperature);
    info += ",";
    Serial.println(info);
  }
  else {
    info += "\"U\":";
    info += "0";
    info += ",";

    info += "\"T\":";
    info += "0";
    info += ",";
    Serial.println(info);
  }

  digitalWrite(D1, 1);
  delayMicroseconds(600);
  double c1 = lerCorrente();
  c1 += lerCorrente();
  c1 = c1 / 2;

  Serial.println("C1:" + String(c1));
  digitalWrite(D1, 0);
  digitalWrite(D3, 1);
  delayMicroseconds(600);
  double c2 = lerCorrente();
  c2 += lerCorrente();
  c2 = c2 / 2;

  Serial.println("C2:" + String(c2));
  digitalWrite(D3, 0);

  info += "\"C1\":";
  info += String(c1);
  info += ",";

  info += "\"C2\":";
  info += String(c2);

  info += "}";
  Serial.println(info);
  server.send(200, "application/json", info);
}

double lerCorrente() {
  float vetCorrente;
  double maior_Valor = 0;
  double valor_Corrente = 0;
  float tensao = 220;
  float potencia = 0;
  for (int i = 0; i < 1000; i++) {
    vetCorrente = analogRead(A0);
    if (maior_Valor < vetCorrente) {
      maior_Valor = vetCorrente;
    }
    delayMicroseconds(600);
  }

  maior_Valor = maior_Valor * 0.004882812;
  valor_Corrente = maior_Valor - 2.5;
  valor_Corrente = valor_Corrente * 1000;
  valor_Corrente = valor_Corrente / 66;         //sensibilidade : 66mV/A para ACS712 30A / 185mV/A para ACS712 5A
  valor_Corrente = valor_Corrente / 1.41421356;
  return valor_Corrente;
}

void ligar1() {
  digitalWrite(D2, 1);
  inicio();
}
void ligar2() {
  digitalWrite(D8, 1);
  inicio();
}
void desligar1() {
  digitalWrite(D2, 0);
  inicio();
}
void desligar2() {
  digitalWrite(D8, 0);
  inicio();
}
