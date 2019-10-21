#include <SPI.h>
#include <LoRa.h>

StaticJsonDocument<200> jsonStatus;

#define BAND 915E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6
#define SCK 5   // GPIO5  SCK
#define MISO 19 // GPIO19 MISO
#define MOSI 27 // GPIO27 MOSI
#define SS 18   // GPIO18 CS
#define RST 14  // GPIO14 RESET
#define DI00 26 // GPIO26 IRQ(Interrupt Request)

void setupLoRa(){ 
  //Inicializa a comunicação
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);

  //Inicializa o LoRa
  if (!LoRa.begin(BAND)){
    //Se não conseguiu inicializar, mostra uma mensagem no display
    logo();
    display.drawString(0, 40, "Erro ao inicializar o LoRa!");
    display.display();
    while (1);
  }

  //Ativa o crc
  LoRa.enableCrc();
  //Ativa o recebimento de pacotes
  LoRa.receive();
}

void enviarLora(StaticJsonDocument<200> dados) {
    String output;
    serializeJson(dados, output);
    String msgSend = criptografar(output);
    Serial.print("Enviando:");
    Serial.println(output);
    mostrarTexto1("S:"+output.substring(0, 30));
    LoRa.beginPacket();
    LoRa.print(msgSend);
    LoRa.endPacket();
}

String receberLora() {
  StaticJsonDocument<200> json;
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    int rssi = LoRa.packetRssi();
    
    String received = "";
    while(LoRa.available()) {
      char c = (char) LoRa.read();
      received += c;
    }
    
    String msgReceived = descriptografar(received);
    
    DeserializationError error = deserializeJson(json, msgReceived.c_str());
     if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return "";
    }
    String origem = json["origem"];
    String destino = json["destino"];
    if (origem && destino && destino == ID) {
      mostrarTexto2("R:"+msgReceived.substring(0, 30));
      mostrarTexto3(String(rssi));
      return msgReceived;
    }
    else if (destino && destino == "*") {
      mostrarTexto3(String(rssi));
    }
  }
  return "";
}
