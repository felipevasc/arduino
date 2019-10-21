//#define MASTER
//#define ID "M1"
#define ID "S1"

#define ID_MASTER "M1"

#include <Wire.h>
#include <ArduinoJson.h>
#include "visor.h";
#include "criptografia.h";
#include "funcoesLora.h";




/* PINO LIGA/DESLIGA BOMBA */
#define BOMBA 35

/* PINOS MONITORAMENTO PRESSAO */
#define PRES_TRIG_PIN 37 //verde
#define PRES_ECHO_PIN 36 //amarelo

/* PINOS MONITORAMENTO AGUA */
#define AGUA_TRIG_PIN 39 //marrom
#define AGUA_ECHO_PIN 38 //cinza
