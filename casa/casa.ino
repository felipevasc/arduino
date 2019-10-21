#include <DNSServer.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <dht.h>

// Nome do seu wifi
char ssid[50];

// Senha do seu wifi
char password[50];
String stringInicio = "<style> span.botao { display: inline-block; width: 200px; height: 50px; background: linear-gradient(to bottom, #33EE33 0%, #00BB00 100%); border-bottom: #399333 solid 7px; text-align: center; vertical-align: middle; line-height: 50px; border-radius: 25px; position: relative; color: #FFF; margin-bottom: 10px; } span.botao.desligado { background: linear-gradient(to bottom, #EE1111 0%, #BB0000 100%); border-bottom: #993333 solid 7px; } span.botao:nth-child(5) { background: linear-gradient(to bottom, #FFFF33 0%, #BBBB00 100%); border-bottom: #999911 solid 7px; } span.botao:nth-child(7) { background: linear-gradient(to bottom, #88FFFF 0%, #44BBBB 100%); border-bottom: #339999 solid 7px; } span.botao:nth-child(9) { background: linear-gradient(to bottom, #FFAA44 0%, #BB8822 100%); border-bottom: #995522 solid 7px; } span.icone { display: inline-block; width: 40px; height: 40px; background-color: #FFF; position: absolute; top: 5px; left: 15px; border-radius: 1000px; box-shadow: inset 3px 3px 10px #aaa; } span.icone img { margin-top: 4px; width: 30px; } span.titulo { display: inline-block; top: -2px; left: 20px; position: absolute; font-weight: bolder; font-size: 20px; } </style> <div style='margin: 0 auto; width: 200px;'> <span class='botao' id='ventilador'> <span class='titulo'>Ventilador</span> </span><br/> <span class='botao' id='janela'> <span class='titulo'>Janela</span> </span><br/> <span class='botao'> <span class='titulo'>Temperatura:<span id='temperatura_v'></span></span> </span><br/> <span class='botao'> <span class='titulo'>Umidade:<span id='umidade_v'></span></span> </span><br/> <span class='botao'> <span class='titulo'>Corrente:<span id='corrente_v'></span></span> </span><br/> </div> <script> function check() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function () { if (this.readyState == 4 && this.status == 200) { var r = this.responseText; var obj = JSON.parse(r); console.log(obj); if (obj['D1']['status'] == 1) { document.getElementById('janela').classList.remove('desligado'); } else { document.getElementById('janela').classList.add('desligado'); } if (obj['D3']['status'] == 1) { document.getElementById('ventilador').classList.remove('desligado'); document.getElementById('ventilador').onclick = function () { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/ledOFF?p=3', true); xhttp.send(); } } else { document.getElementById('ventilador').classList.add('desligado'); document.getElementById('ventilador').onclick = function () { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/ledON?p=3', true); xhttp.send(); } } if (typeof obj['umidade'] != undefined && obj['umidade'] != null) { document.getElementById('umidade_v').innerHTML = obj['umidade']; } if (typeof obj['temperatura'] != undefined && obj['temperatura'] != null) { document.getElementById('temperatura_v').innerHTML = obj['temperatura']; } if (typeof obj['corrente'] != undefined && obj['corrente'] != null) { document.getElementById('corrente_v').innerHTML = obj['corrente']; } setTimeout(function () { check(); }, 1000) } }; xhttp.open('GET', '/pinos', true); xhttp.send(); } window.onload = function () { check(); } </script>";

ESP8266WebServer server(80);

void inicio() {
  String message = "";
  message = htmlInicio();
  server.send(200, "text/html", message);
}
String htmlInicio() {
  return stringInicio;
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

int convertePino(int i) {
  int pino;
  if (i == 0) {
    pino = D0;
  }
  else if (i == 1) {
      pino = D1;
    }
    else if (i == 2) {
      pino = D2;
    }
    else if (i == 3) {
      pino = D3;
    }
    else if (i == 4) {
      pino = D4;
    }
    else if (i == 5) {
      pino = D5;
    }
    else if (i == 6) {
      pino = D6;
    }
    else if (i == 7) {
      pino = D7;
    }
    else if (i == 8) {
      pino = D8;
    }
    return pino;
}
void pinos() {
  String message = "{";
  int pino;
  int status;
  for (int i = 1; i < 9; i++) {
    pino = convertePino(i);
    status = digitalRead(pino);
    message += "\"D"+String(i)+"\":{\"pino\":\""+String(pino)+"\",\"status\":\""+String(status)+"\"},";
  }
  

  
  dht DHT;
  #define DHT11_PIN D7
  int chk = DHT.read11(DHT11_PIN);
  if(chk == DHTLIB_OK) {
    message += "\"umidade\":\""+String(DHT.humidity)+"\", \"temperatura\":\""+String(DHT.temperature)+"\",";
    Serial.print("umidade: ");
    Serial.print(DHT.humidity);
    Serial.print("    Temperatura: ");
    Serial.println(DHT.temperature);
  }
  else {
    Serial.print("Temp N");
  }
  

  float vetCorrente[300];
  double maior_Valor = 0;
  double valor_Corrente = 0;
  float tensao = 220;
  float potencia = 0;
  for(int i = 0; i < 300; i++)
  {
    vetCorrente[i] = analogRead(A0);
    delayMicroseconds(600);
  }   
  for(int i = 0; i < 300; i++)
  {
    if(maior_Valor < vetCorrente[i])
    {
      maior_Valor = vetCorrente[i];
    }
  }  
  maior_Valor = maior_Valor * 0.004882812;
  valor_Corrente = maior_Valor - 2.5;
  valor_Corrente = valor_Corrente * 1000;
  valor_Corrente = valor_Corrente / 66;         //sensibilidade : 66mV/A para ACS712 30A / 185mV/A para ACS712 5A
  valor_Corrente = valor_Corrente / 1.41421356;
  message += "\"corrente\":\""+String(valor_Corrente)+"\",";
  potencia = valor_Corrente * tensao;
  message += "\"potencia\":\""+String(potencia)+"\",";

   
  message += "\"ok\":\"ok\"";
  message += "}";
  server.send(200, "text/html", message);
}
void ledON() {
    int pino;
    int i = server.arg(0).toInt();
    pino = convertePino(i);
    digitalWrite(pino, 1);
    server.send(200, "text/html", "OK");
}

void ledOFF() {
    int pino;
    int i = server.arg(0).toInt();
    pino = convertePino(i);
    digitalWrite(pino, 0);
    server.send(200, "text/html", "OK");
}

void conectar() { 
  server.arg(0).toCharArray(ssid, 50);
  server.arg(1).toCharArray(password, 50);
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
    select += "<label style='display: inline-block; width: 98%; margin: 0 auto; font-size: 16px; padding: 4px; text-align: left; background-color: "+cor+"; border-radius: 4px; margin: 2px;'><input name='rede' type='radio' value='"+String(WiFi.SSID(i))+"'> " + String(WiFi.SSID(i)) + " (" + String(qualidade(WiFi.RSSI(i))) + "%)</label>";  
  }
  select += "</div>";
  
  message += html();
  message.replace("[REDES]", select);
  server.send(200, "text/html", message);
}

void setup() {
  // Para o node mcu esp8266 a velocidade do monitor serial e 115200
  Serial.begin(115200);
  delay(10);

  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
  digitalWrite(D6, 0);
  pinMode(D7, OUTPUT);
  digitalWrite(D7, 0);
  pinMode(D8, INPUT);
  pinMode(A0, INPUT);


  // Inicializa o servidor (nesse caso o proprio esp8266)
  /*
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.persistent(true);
  */
  /*
  WiFi.persistent(false);
  // disconnect sta, start ap
  WiFi.disconnect(); //  this alone is not enough to stop the autoconnecter
  //WiFi.mode(WIFI_AP);
  //WiFi.mode(WIFI_AP_STA);
  //WiFi.mode(WIFI_STA);
  WiFi.persistent(true);
  
  WiFi.softAP("Teste");
  bool ok = WiFi.mode(WIFI_AP_STA); */
  WiFi.softAP("FVS");
  //conectar();
  
  delay(1000);
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.waitForConnectResult());

  server.begin();
  Serial.println("Servidor inicializado"); 
  server.on("/", inicio); 
  server.on("/configurar", configurar); 
  server.on("/conectar", conectar); 
  server.on("/ledON", ledON); 
  server.on("/ledOFF", ledOFF); 
  server.on("/pinos", pinos); 
  Serial.println("Conectar");
  conectar();
}


void loop() {
  server.handleClient(); 
}
