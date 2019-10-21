//Compila apenas se MASTER não estiver definido no arquivo principal
#ifndef MASTER

void setup(){
    Serial.begin(115200);
    //Chama a configuração inicial do display
    pinMode(LED, OUTPUT);
    pinMode(LIGAR, INPUT);
    pinMode(ALTO, INPUT);
    pinMode(BAIXO, INPUT);
    setupDisplay();
    //Chama a configuração inicial do LoRa
    setupLoRa();
    mostrarTexto("Slave", "Aguardando conexao");
    mostrar();
}

void loop(){
  //Tenta ler o pacote
  int packetSize = LoRa.parsePacket();
  mostrarTexto3(String(LoRa.packetRssi()));
  //Verifica se o pacote possui a quantidade de caracteres que esperamos
  if (packetSize > 2){
    String received = "";

    //Armazena os dados do pacote em uma string
    while(LoRa.available()){
      received += (char) LoRa.read();
    }
    mostrarTexto2("R:"+received);
    Serial.println("Criando pacote para envio");
    int ligar = digitalRead(LIGAR);
    int index = received.indexOf("led:");
    if (index >= 0) {
      Serial.println(received.substring(index + 4, 1));
      led = received.substring(index + 4, index + 5).toInt();
      digitalWrite(LED, led);
    }
    
    
    int alto = digitalRead(ALTO);
    int baixo = digitalRead(BAIXO);
    String data = "{alto:"+String(alto)+",baixo:"+String(baixo)+"}";
    
    //Cria o pacote para envio
    LoRa.beginPacket();
    LoRa.print(data);
    //Finaliza e envia o pacote
    LoRa.endPacket();
    //Mostra no display
    mostrarTexto1("S: " + String(data));
  }
  mostrar();
}

#endif
