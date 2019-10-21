#include <DNSServer.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


// Nome do seu wifi
const char* ssid = "fel";
const char* conectado = "";

// Senha do seu wifi
const char* password = "vasconcelos";

// Porta de comunicacao (normalmente se utiliza a 80 ou 8080)
ESP8266WebServer server(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;
void setup() {
//  dnsServer.reset(new DNSServer());
  // Para o node mcu esp8266 a velocidade do monitor serial e 115200
  Serial.begin(115200);
  delay(10);
 
  pinMode(5, INPUT);
  digitalWrite(5, 0);
  pinMode(4, INPUT);
  digitalWrite(4, 0);


  // Inicializa o servidor (nesse caso o proprio esp8266)
  server.begin();
  Serial.println("Servidor inicializado");
  
  //WiFi.persistent(false);
  // disconnect sta, start ap
  //WiFi.disconnect(); //  this alone is not enough to stop the autoconnecter
  //WiFi.mode(WIFI_AP);
  //WiFi.mode(WIFI_AP_STA);
  //WiFi.mode(WIFI_STA);
  WiFi.persistent(true);
  
  WiFi.softAP("Teste");
  bool ok = WiFi.mode(WIFI_AP_STA);
  if (ok) {
    Serial.println("WIFI Modo AP_STA");
  }
  else {
    Serial.println("N Mudou o modo WIFI");
  }
  delay(1000);
  
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  Serial.println("Dados:");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.waitForConnectResult());

  WiFi.begin(ssid, password);
  int x = 0;
  while (WiFi.status() != WL_CONNECTED && x < 80) {
    delay(1000);
    Serial.print("."+WiFi.status());
    Serial.printf("Connection status: %d\n", WiFi.status());
    x++;
  }
  
  server.on("/configurar", configurar); 
  server.on("/conectar", conectar); 
  Serial.println(WiFi.localIP());
}

void conectar() {
  Serial.println("Inicio");
  Serial.println("WiFi "+WiFi.status());
  WiFi.persistent(false);
  WiFi.disconnect();
  bool ok = WiFi.mode(WIFI_AP_STA);
  if (ok) {
    Serial.println("Mudou pro mod ap_sta");
  }
  else {
    Serial.println("N Mudou pro mod ap_sta");
  }
  WiFi.persistent(true);
  delay(1000);
  WiFi.begin(ssid, password);
  int x = 0;
  while (WiFi.status() != WL_CONNECTED && x < 80) {
    delay(1000);
    Serial.print("."+WiFi.status());
    Serial.printf("Connection status: %d\n", WiFi.status());
    x++;
  }

Serial.println("WiFi "+WiFi.status());
  WiFi.persistent(false);
  WiFi.disconnect();
  ok = WiFi.mode(WIFI_STA);
  if (ok) {
    Serial.println("Mudou");
  }
  else {
    Serial.println("N Mudou");
  }
  WiFi.persistent(true);
  delay(1000);
  WiFi.begin(ssid, password);
  x = 0;
  while (WiFi.status() != WL_CONNECTED && x < 80) {
    delay(1000);
    Serial.print("."+WiFi.status());
    Serial.printf("Connection status: %d\n", WiFi.status());
    x++;
  }


  
  Serial.println("WiFi connectado");
  String message = "Conectado: "+WiFi.localIP();
  server.send(200, "text/html", message);
}

void configurar() {
  String message = "Página de Configuração <input type='text'/><hr/>";
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i)+":";
    message += server.arg(i)+"<hr/>";
  }
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    message += String(i) + " - " + String(WiFi.SSID(i)) + " (" + String(qualidade(WiFi.RSSI(i))) + "%) <hr/>";  
  }
  server.send(200, "text/html", message);
}

int qualidade(int RSSI) {
  int quality = 0;
  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

void loop() {
  server.handleClient(); 
  /*
  if (ssid != "" && conectado == "") {
      // Inicializando a conexao
      WiFi.begin(ssid, password);
     
      // Enquanto nao conseguir conectar imprime um ponto na tela (dá a ideia de que esta carregando) 
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
  }
      
 
  // Guarda o status do servidor
  WiFiClient client = server.available();
  if ( ! client) {
    return;
  }
 
  // Quando estiver alguem acessando
  Serial.println("novo cliente");
 
  // Enquanto nao tiver cliente
  while ( ! client.available()) {
    delay(1);
  }
 
  // Lê caracteres do buffer serial
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
  // Verifica se existe a substring led5_on
  if (req.indexOf("led5_on") != -1) {
    digitalWrite(5, 1);
    Serial.println(req.indexOf("led5_on"));
   
  } else if (req.indexOf("led5_off") != -1) {
    digitalWrite(5, 0);
    Serial.println(req.indexOf("led5_off"));
  }

  if (req.indexOf("led4_on") != -1) {
    digitalWrite(4, 1);
    Serial.println(req.indexOf("led4_on"));
   
  } else if (req.indexOf("led4_off") != -1) {
    digitalWrite(4, 0);
    Serial.println(req.indexOf("led4_off"));
  }

  client.flush();
  String buf = "";
  buf += "<html><head>";
  buf += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  buf += "<title>ESP8266 Web Server</title>";
  buf += "</head>";
  buf += "<form method='GET'><input name='rede' type='text'/><hr/><input name='senha' type='text'/></form>";
  buf += "</html>";
  client.print(buf);
  client.flush();
  
   Serial.println("Cliente desconectado"); */
}
/*

int connectWifi(String ssid, String pass) {
  server.println("conectando ao wifi");

  //WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn);
    
  if (WiFi.status() == WL_CONNECTED) {
    server.println("Já está conectado.");
    return WL_CONNECTED;
  }
  
  //check if we have ssid and pass and force those, if not, try with last saved values
  if (ssid != "") {
    WiFi.begin(ssid.c_str(), pass.c_str());
  } else {
    if (WiFi.SSID()) {
      DEBUG_WM(F("Using last saved values, should be faster"));
      //trying to fix connection in progress hanging
      ETS_UART_INTR_DISABLE();
      wifi_station_disconnect();
      ETS_UART_INTR_ENABLE();

      WiFi.begin();
    } else {
      DEBUG_WM(F("No saved credentials"));
    }
  }

  int connRes = WiFi.waitForConnectResult();
  DEBUG_WM ("Connection result: ");
  DEBUG_WM ( connRes );
  //not connected, WPS enabled, no pass - first attempt
  

  if (connRes != WL_CONNECTED) {
    WiFi.beginWPSConfig();
    connRes = WiFi.waitForConnectResult();
  }
  #endif
  return connRes;
} */
