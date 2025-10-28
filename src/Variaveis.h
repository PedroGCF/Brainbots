enum ladoSensor {ESQ = 4, DIR = 7};
enum ladoSensorUltra {ESQu, DIRu, FRENTEu}; // Valor dos parâmetros implícito (ESQ = 0, DIR = 1, FRENTE = 2)
enum ladoSensorTof {ESQt = 3, DIRt = 2, FRENTEt = 1};
enum corSensor {LUZ, RED, GREEN, BLUE}; // Valor dos parâmetros implícito (LUZ = 0, RED = 1, GREEN = 2, BLUE = 3)

//Sensores Infra
int in_preto = 60;

//Prata
int prata = 850;  // maximo 1023 // apds 1025

//Sensor APDS
int apds_preto_esq = 5000;
int apds_preto_dir = 6000;

// Proporção verde/vermelho: para aumentar a sensibilidade diminua o valor
float esqGR = 1.8;  
float dirGR = 1.5; // 1.7

// Proporção vermelho: para aumentar a sensibilidade, aumente o valor
float red_calib = 1.0; 
