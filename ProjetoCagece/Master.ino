//Compila apenas se MASTER estiver definido no arquivo principal
#ifdef MASTER
String S1, S2, S3, S4, S5, S6;
long reservatorio;

int key[] = {0, 0, 0, 0, 0, 0};

#include "funcoesWifi.h";

//Intervalo entre os envios
#define INTERVAL 1000
long lastSendTime = 0;


void setup(){
  Serial.begin(115200);
  //Chama a configuração inicial do display
  setupDisplay();
  //Chama a configuração inicial do LoRa
  setupLoRa();
  
  //pinMode(LED, OUTPUT);
  //pinMode(LIGAR, INPUT);
  mostrarTexto("Master ", "Aguardando conexao");
  mostrar();
  
  conectarWifi();
}

void loop(){
 
  if (millis() - lastSendTime > INTERVAL){
    lastSendTime = millis();

    reservatorio = getDistance(RESERVATORIO_TRIG_PIN, RESERVATORIO_ECHO_PIN);
    if (reservatorio < 4) {
      jsonStatus["key"] = key[0];
      jsonStatus["bomba"] = 1;
      jsonStatus["destino"] = "S"+String(i + 1);
      jsonStatus["origem"] = ID; 
      enviarLora(jsonStatus);
    }
    if (reservatorio > 300) {
      jsonStatus["key"] = key[0];
      jsonStatus["bomba"] = 0;
      jsonStatus["destino"] = "S"+String(i + 1);
      jsonStatus["origem"] = ID; 
      enviarLora(jsonStatus);
    }
    
    for (int i = 0; i < 6; i++) {
      if (bomba[i] < 2) {
        jsonStatus["key"] = key[i];
        jsonStatus["bomba"] = bomba[i];
        jsonStatus["destino"] = "S"+String(i + 1);
        jsonStatus["origem"] = ID; 
        enviarLora(jsonStatus);
      } 
    }
  }

  String msgRecebida = receberLora();
  
  if (msgRecebida &&  msgRecebida  != "") {
    Serial.println(msgRecebida);
    StaticJsonDocument<200> json;
    DeserializationError error = deserializeJson(json, msgRecebida.c_str());
    if (!error) {
      String origem = json["origem"];
      if (origem == "S1") {
          S1 = msgRecebida;
          key[0] = json["key"];
      }
      else if (origem == "S2") {
          S2 = msgRecebida;
          key[1] = json["key"];
      }
      else if (origem == "S3") {
          S3 = msgRecebida;
          key[2] = json["key"];
      }
      else if (origem == "S4") {
          S4 = msgRecebida;
          key[3] = json["key"];
      }
      else if (origem == "S5") {
          S5 = msgRecebida;
          key[4] = json["key"];
      }
      else if (origem == "S6") {
          S6 = msgRecebida;
          key[5] = json["key"];
      }
    }
  }
  mostrar();
  server.handleClient();
}

#endif
