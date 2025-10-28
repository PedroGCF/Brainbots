#include <Wire.h> // Comunicação Serial - I2C
#include <SparkFun_APDS9960.h> // Comunicação APDS9960 - Cor
#include <Adafruit_TCS34725.h> // Comunicação TCS34725 - Cor
#include <NeoSWSerial.h> // Comunicação Serial - Raspberry
#include <Adafruit_Sensor.h> // Interface Genérica - Controle de sensores
#include <Adafruit_BNO055.h> // Sensor BNO - Orientação Absoluta
#include <utility/imumaths.h> // Funções Matemáticas - Vetoriais
#include <math.h> // Funções Matemáticas
#include <Servo.h> // Controle de Servomotores
#include <VL53L0X.h> // Controle de ToF
#include <EEPROM.h> // Memória do Arduino

#include "Pinos.h"
#include "Variaveis.h"
#include "fCustom.h"
#include "Movimentacao.h"
#include "LEDs.h" // Funções para controlar os LEDs do robô
#include "Genericos.h"
#include "SLinha.h"
#include "Com_Serial.h" // Gerencia o envio e recebimento de dados via serial
#include "Desvia_Obs.h"
#include "PID.h"
#include "Verde.h"
#include "Curva90.h"
#include "MUV_Lado.h"
#include "MUV_Subida.h"
#include "Imprimir.h"
#include "SDR_Funcoes.h"
#include "SDR_Saida.h"
#include "SDR_Triangulo.h"
#include "SDR_Vitimas.h"
#include "SDR_Setups.h"
#include "SDR.h"
#include "Setup.h"
#include "Principal.h"

void setup() {
  subSetup();
}
void loop() {
  subLoop();
}
