//#define MASTER
//#define ID "M1"
#define ID "S2"

#define ID_MASTER "M1"

#include <Wire.h>
#include <ArduinoJson.h>
#include "visor.h";
#include "criptografia.h";
#include "funcoesLora.h";




/* PINO LIGA/DESLIGA BOMBA */
#define BOMBA 22

/* PINOS MONITORAMENTO RESERVATORIO MASTER */
#define RESERVATORIO_TRIG_PIN 39 //verde
#define RESERVATORIO_ECHO_PIN 38 //amarelo

/* PINOS MONITORAMENTO AGUA */
#define AGUA_TRIG_PIN 39 //marrom
#define AGUA_ECHO_PIN 38 //cinza
