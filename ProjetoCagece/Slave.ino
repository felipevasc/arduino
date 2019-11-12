//Compila apenas se MASTER não estiver definido no arquivo principal
#ifndef MASTER


#include "sensorUltrasonico.h";
#define INTERVAL 3000

long lastSendTime = 0;
long lastChangeKey = 0;
int key = 1;
void setup(){
    Serial.begin(115200);
    //Chama a configuração inicial do display
    pinMode(BOMBA, OUTPUT);
    setupDisplay();
    //Chama a configuração inicial do LoRa
    setupLoRa();
    mostrarTexto("Slave", "Aguardando conexao");
    mostrar();
    Serial.println("Iniciado");
}

void loop() {
  if (millis() - lastChangeKey > 120000) {
    lastChangeKey = millis();
    key = random(1, 999999);
  }
  if (millis() - lastSendTime > INTERVAL){
    lastSendTime = millis();
    
    jsonStatus["agua"] = getDistance(AGUA_TRIG_PIN, AGUA_ECHO_PIN);
    jsonStatus["pressao"] = getDistance(RESERVATORIO_TRIG_PIN, RESERVATORIO_ECHO_PIN);
    jsonStatus["bomba"] = digitalRead(BOMBA);
    jsonStatus["key"] = key;
    jsonStatus["origem"] = ID;
    jsonStatus["destino"] = ID_MASTER;
    enviarLora(jsonStatus);
  }
  
  String recebida = receberLora();  
  if (recebida.indexOf("\"key\":"+String(key)) >= 0) {
    Serial.println("EVENTO BOMBA");
    if (recebida.indexOf("\"bomba\":1") >= 0) {
      digitalWrite(BOMBA, 1);
      Serial.println("LIGAR BOMBA");
    }
    else if (recebida.indexOf("\"bomba\":0") >= 0) {
      digitalWrite(BOMBA, 0);
      Serial.println("DESLIGAR BOMBA");
    }
  }
  mostrar();
}

#endif
