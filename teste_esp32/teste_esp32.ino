#include "heltec.h" 
#include "images.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawXbm(0,0,logo_width,logo_height,logo_bits);
  Heltec.display->drawString(0 , 55 , "Recebido "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 76 , 128, packet);
  Heltec.display->drawString(0, 40, rssi);
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { 
    packet += (char) LoRa.read(); 
  }
  rssi = "RSSI: " + String(LoRa.packetRssi(), DEC) ;
  LoRaData();
}

void setup() { 
  Serial.begin(115200);
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  Serial.println("Inicio");
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 40, "Iniciado!");
  Heltec.display->drawString(0, 50, "Aguardando pacote...");
  Heltec.display->drawXbm(0,0,logo_width,logo_height,logo_bits);
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  Serial.println("Receber");
  LoRa.receive();
  Serial.println("fim setup");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  Serial.println(packetSize);
  if (packetSize) { 
    cbk(packetSize);  
    Serial.println(packetSize);
  }
  delay(10);
}
