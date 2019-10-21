#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <SSD1306.h>
#include "images.h"

//Deixe esta linha descomentada para compilar o Master
//Comente ou remova para compilar o Slave
#define MASTER

#define SCK 5   // GPIO5  SCK
#define MISO 19 // GPIO19 MISO
#define MOSI 27 // GPIO27 MOSI
#define SS 18   // GPIO18 CS
#define RST 14  // GPIO14 RESET
#define DI00 26 // GPIO26 IRQ(Interrupt Request)

#define BAIXO 32
#define LED 22
#define ALTO 25
#define LIGAR 34

int baixo, led, alto, ligar;

#define BAND 915E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6

//Variável para controlar o display
SSD1306 display(0x3c, 4, 15);

String textoParte1 = "";
String textoParte2 = "";
String textoParte3 = "";

void logo(){
  display.clear();
  display.drawXbm(0,0,logo_width,logo_height,logo_bits);
  display.setFont(ArialMT_Plain_24);
  display.drawString(40, 10, "Cagece");
  display.setFont(ArialMT_Plain_10);
}
void mostrarTexto1(String texto1) {
  textoParte1 = texto1;
}
void mostrarTexto2(String texto2) {
  textoParte2 = texto2;
}
void mostrarTexto3(String texto3) {
  textoParte3 = texto3;
}
void mostrarTexto(String texto1, String texto2) {
  logo();
  mostrarTexto1(texto1);
  mostrarTexto2(texto2);
}
void mostrar() {
  logo();
  display.drawString(0, 40, textoParte1);
  display.drawString(0, 50, textoParte2);
  display.setFont(ArialMT_Plain_10);
  display.drawString(100, 0, textoParte3);
  display.display();
}

void setupDisplay(){
  //O estado do GPIO16 é utilizado para controlar o display OLED
  pinMode(16, OUTPUT);
  //Reseta as configurações do display OLED
  digitalWrite(16, LOW);
  //Para o OLED permanecer ligado, o GPIO16 deve permanecer HIGH
  //Deve estar em HIGH antes de chamar o display.init() e fazer as demais configurações,
  //não inverta a ordem
  digitalWrite(16, HIGH);

  //Configurações do display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

//Configurações iniciais do LoRa
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
