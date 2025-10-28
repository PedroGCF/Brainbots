void apdsSetup();
void apdsIni(int lado); // Inicializa o apds
void apdsPlex(int lado); // Multiplexamento dos dois apds
uint16_t sCor(ladoSensor lado, corSensor cor); // Leitura do apds - sCor (Lado do sensor, Cor da leitura) {ESQ, DIR} {LUZ, RED, GREEN, BLUE}
boolean ehVermelho(ladoSensor lado); // Retorna se é vermelho ou não (Lado do sensor) {ESQ, DIR}
boolean ehVerde(ladoSensor lado); // Retorna se é verde ou não (Lado do sensor) {ESQ, DIR}
void verde(); // Desvia do verde e chama curva de 90 [Função mestre]
void verdeEsq(); // Desvia do verde para a esquerda
void verdeDir(); // Desvia do verde para a direita
void verdeDois(); // Desvia do verde duplo

void curva90(); // Definição cruzada com o Curva90.h
void PID();
bool encoderVerde();
void encoderVerdeReset();
void resetPid();// Definição cruzada com o MUV_Lado.h

SparkFun_APDS9960 apds = SparkFun_APDS9960(); // Define o sensor

uint16_t ambient_light = 0; // Define algo do apds
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

byte V90;

extern long imuEncoder90_2;

/*
 * Pinos I2C? (mega):
 * SDA(Serial Data) - 20
 * SCL(Serial Clock) - 21
 */

void apdsSetup(){
  apdsIni(ESQ); // Inicializa os dois sensores
  apdsIni(DIR);
}

void apdsIni(int lado){
  apdsPlex(lado);
  apds.init();
  apds.enableLightSensor(false);
  apds.setAmbientLightGain(AGAIN_16X);
}

void apdsPlex(int lado){
  Wire.beginTransmission(0x73);
  Wire.write(1 << lado);
  Wire.endTransmission();
}

uint16_t sCor(ladoSensor lado, corSensor cor){
  apdsPlex(lado);
  apds.isAlsAvailable();
  switch (cor) {
    case LUZ:
      apds.readAmbientLight(ambient_light);
      return ambient_light;
    case RED:
      apds.readRedLight(red_light);
      return red_light;
    case GREEN:
      apds.readGreenLight(green_light);
      return green_light;
    case BLUE:
      apds.readBlueLight(blue_light);
      return blue_light;
    default:
      return 0;
  }
}

boolean ehVermelho(ladoSensor lado){
  apdsPlex(lado);
  apds.isAlsAvailable(); // Codigozinho personalizado do Walisson
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  float proporcao = (float)green_light / (float)red_light;
  return proporcao < red_calib; // Isso é basicamente uma comparação que retorna se é verdadeira ou não
}

boolean ehVerde(ladoSensor lado){
  apdsPlex(lado);
  apds.isAlsAvailable(); // Codigozinho personalizado do Walisson
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  float proporcao = (float)green_light / (float)red_light;
  /*
  Serial.print(" ");
  Serial.print(proporcao);
  Serial.println(" ");
  */
  if(red_light < 20){ return false;}
  if(lado = ESQ){ return proporcao > esqGR;}else{ return proporcao > dirGR;}
}

void verde(){
  limpaLeds();
  bool esqGreen = ehVerde(ESQ);
  bool dirGreen = ehVerde(DIR);
  if(esqGreen){
    if(dirGreen){
      moveMotores(100, 100);
      delay(100);
      verdeDois();
    } else{ // esqGreen && !dirGreen
      moveMotores(60, 60);
      bool doisVerdes = false;
      repitaPorTempo(75){ // 100
        bool dirGreen = ehVerde(DIR);
        if(dirGreen){
          doisVerdes = true;
        }
      }
      Stop();
      if(doisVerdes){
        verdeDois();
      }
      else{
        verdeEsq();
      }
    }
  } else{
    if(dirGreen){ // dirGreen && !esqGreen
      moveMotores(60, 60);
      bool doisVerdes = false;
      repitaPorTempo(75){
        bool esqGreen = ehVerde(ESQ);
        if(esqGreen){
          doisVerdes = true;
        }
      }
      Stop();
      if(doisVerdes){
        verdeDois();
      }
      else{
        verdeDir();
      }
    }
    else{
      comI = true;
      PID();
    }
  }
}

void verdeEsq(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  bool arrumar = true;
  if(PID_dois_verdes > millis()){arrumar = false;}
  /*
  repitaPorTempo(300){
    PID();
  }
  */
  tempo1 = 1;
  tempo2 = 0;
  encoderVerdeReset();
  comLimite = true;
  while(!encoderVerde()){ // Continua seguindo a linha enquanto avança
    PID();
  }
  comLimite = false;
  resetPid();
  
  //moveMotores(100, -100);
  //delay(150);
  moveMotores(-120, 120); // 100
  while(reLeitura(infra, 0, in_preto, 5));
  delay(50);
  while(!reLeitura(infra, 0, in_preto, 5));
  delay(50);
  while(reLeitura(infra, -1, in_preto, 5));
  delay(150);
  if(arrumar){//PID_alt = millis() + 500;
    } // 750
  else{
    moveMotores(-100, -100);
    delay(150);
    Stop();
    delay(500);
  }
  imuVerde = millis() + 250;
  imu90 = millis() + 250;
  imuRec = millis() + 250;
  imuEncoder90_2 = millis() + 5000;
  PID_dois_verdes = millis() + 1000;
  PID_esq = true;
  digitalWrite(led_esq_G, LOW);
}

void verdeDir(){
  limpaLeds();
  digitalWrite(led_dir_G, HIGH);
  bool arrumar = true;
  if(PID_dois_verdes > millis()){arrumar = false;}
  /*
  repitaPorTempo(300){
    PID();
  }
  */
  tempo1 = 1;
  tempo2 = 0;
  encoderVerdeReset();
  comLimite = true;
  while(!encoderVerde()){
    PID();
  }
  comLimite = false;
  resetPid();
  
  //moveMotores(-100, 100);
  //delay(150);
  moveMotores(120, -120);
  while(reLeitura(infra, 15, in_preto, 5));
  delay(50);
  while(!reLeitura(infra, 15, in_preto, 5));
  delay(50);
  while(reLeitura(infra, -1, in_preto, 5));
  delay(150);
  if(arrumar){//PID_alt = millis() + 500;
    }
  else{
    moveMotores(-100, -100);
    delay(150);
    Stop();
    delay(500);
  }
  imuVerde = millis() + 250;
  imu90 = millis() + 250;
  imuRec = millis() + 250;
  imuEncoder90_2 = millis() + 5000;
  PID_dois_verdes = millis() + 1000;
  PID_esq = false;
  digitalWrite(led_dir_G, LOW);
}

void verdeDois(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
  //delay(150);
  MRUAngular(135, 120);
  while(infra(-1) > in_preto){
    moveMotores(120, -120);
  }
  //delay(150);
  //imuVerde = millis() + 250;
  imuRec = millis() + 250;
  digitalWrite(led_esq_G, LOW);
  digitalWrite(led_dir_G, LOW);
}
