//Compila apenas se MASTER não estiver definido no arquivo principal
#ifndef MASTER


#include "sensorUltrasonico.h";
#define INTERVAL 5000

long lastSendTime = 0;
long lastChangeKey = 0;
int key = 1;
int valorBomba = 0;
void setup(){
    Serial.begin(115200);
    //Chama a configuração inicial do display
    pinMode(BOMBA, OUTPUT);
    digitalWrite(BOMBA, 1);
    setupDisplay();
    //Chama a configuração inicial do LoRa
    setupLoRa();
    mostrarTexto("Slave", "Aguardando conexao");
    mostrar();
    Serial.println("Iniciado");
}

void loop() {
  String recebida = receberLora();  
  if (millis() - lastChangeKey > 120000) {
    lastChangeKey = millis();
    key = random(1, 999999);
  }
  if (millis() - lastSendTime > INTERVAL){
    lastSendTime = millis();
    
    jsonStatus["agua"] = getDistance(AGUA_TRIG_PIN, AGUA_ECHO_PIN);
    jsonStatus["pressao"] = getDistance(RESERVATORIO_TRIG_PIN, RESERVATORIO_ECHO_PIN);
    jsonStatus["bomba"] = valorBomba;
    jsonStatus["key"] = key;
    jsonStatus["origem"] = ID;
    jsonStatus["destino"] = ID_MASTER;
    enviarLora(jsonStatus);
  }
  
  
  if (recebida.indexOf("\"key\":"+String(key)) >= 0) {
    Serial.println("EVENTO BOMBA");
    if (recebida.indexOf("\"bomba\":1") >= 0) {
      valorBomba = 1;
      digitalWrite(BOMBA, HIGH);
      Serial.println("LIGAR BOMBA");
    }
    else if (recebida.indexOf("\"bomba\":0") >= 0) {
      valorBomba = 0;
      digitalWrite(BOMBA, LOW);
      Serial.println("DESLIGAR BOMBA");
    }
  }
  mostrar();
}

#endif
