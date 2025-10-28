void curva90(); // Desvia de curva de 90 e chama o verde [função secundária]
void curva90Rec(); // Sub-90 para a jogadinha
void reTeste(); // Jogadinha para achar o verde
void encoder90(); // Imunidade90 por encoder
bool encoderVerde(); // Ir para frente verde
void encoder90Reset(); // Reseta o encoder
void encoderVerdeReset(); // Reseta o encoder verde
void curva90Esq(); // Desvia de curva de 90 para a ESQuerda
void curva90Dir(); // Desvia de curva de 90 para a direita

void verde(); // Definição cruzada com o Verde.h
void verde90(); // Codigos_Genéricos.h
void PID();

long coder_esq_90 = 0;
long coder_dir_90 = 0;
long dif_esq = 0;
long dif_dir = 0;
long coder_esq_verde = 0;
long coder_dir_verde = 0;
long dif_esq_verde = 0;
long dif_dir_verde = 0;

extern byte V90; // Declara uma variavel global
extern bool semVerde;
extern long imuVerde;
extern long imu90;
extern bool imuEncoder90;



void curva90(){
  // Se o sensor esquerdo ou o sensor direito lerem preto
  if(reLeitura(infra, 0, in_preto, 50) || reLeitura(infra, 15, in_preto, 50)){
    imuVerde = millis() + 250;
  }
}

void curva90Rec(){
  limpaLeds();
  if(reLeitura(infra, 0, in_preto, 100)){
    if(reLeitura(infra, 15, in_preto, 100)){
      reTeste();
    }
    else{
      lado = 0;
      validador1 = false;
      validador2 = true;
      reTeste();
    }
  }
  else{
    if(reLeitura(infra, 15, in_preto, 100)){
      lado = 1;
      validador1 = true;
      validador2 = false;
      reTeste();
    }
    else{
      
    }
  }
}

void reTeste(){
  moveMotores(-100, -100);
  delay(250);
  moveMotores(160, -200);
  delay(75);
  bool temp_verde_esq = false;
  bool temp_verde_dir = false;
  for(int i = 0; i < 2; i++){
    moveMotores(-200, 160);
    repitaPorTempo(100){
      if(ehVerde(ESQ)){ temp_verde_esq = true;}
      if(ehVerde(DIR)){ temp_verde_dir = true;}
    }
    Stop();
    delay(100);
    moveMotores(160, -200);
    repitaPorTempo(100){
      if(ehVerde(ESQ)){ temp_verde_esq = true;}
      if(ehVerde(DIR)){ temp_verde_dir = true;}
    }
    Stop();
    delay(100);
  }
  moveMotores(-200, 160);
  delay(50);
  Stop();
  if(temp_verde_esq){
    if(temp_verde_dir){
      moveMotores(100, 100);
      delay(100);
      verdeDois();
      imuVerde = millis() + 250;
      imu90 = millis() + 250;
    }
    else{
      moveMotores(100, 100);
      delay(200);
      verdeDir();
      imuVerde = millis() + 250;
      imu90 = millis() + 250;
    }
  }
  else{
    if(temp_verde_dir){
      moveMotores(100, 100);
      delay(200);
      verdeEsq();
      imuVerde = millis() + 250;
      imu90 = millis() + 250;
    }
    else{
      Stop();
      delay(100);
      moveMotores(100, 100);
      delay(150);
      Stop();
      encoder90Reset();
      imuVerde = millis() + 500;
      imu90 = millis() + 500;
      tempoNoLimite = millis() + 500;
    }
  }
}

void encoder90(){
  noInterrupts();
  long tempEsq = encoderEsq;
  long tempDir = encoderDir;
  interrupts();
  long dif_esq_temp = tempEsq - coder_esq_90;
  long dif_dir_temp = tempDir - coder_dir_90;
  coder_esq_90 = tempEsq;
  coder_dir_90 = tempDir;
  if(dif_esq_temp < 0){ dif_esq += dif_esq_temp * 10;}else{ dif_esq += dif_esq_temp;}
  if(dif_dir_temp < 0){ dif_dir += dif_dir_temp * 10;}else{ dif_dir += dif_dir_temp;}
  if(dif_esq < 0){ dif_esq = 0;}
  if(dif_dir < 0){ dif_dir = 0;}
  int dif = dif_esq + dif_dir;
  Serial.print(" ");
  Serial.print(dif);
  Serial.println(" ");
  if(dif > 1000){
    imuEncoder90 = true;
  }
  else{
    imuEncoder90 = false;
  }
}

bool encoderVerde(){
  noInterrupts();
  long tempEsq = encoderEsq;
  long tempDir = encoderDir;
  interrupts();
  long dif_esq_temp = tempEsq - coder_esq_verde;
  long dif_dir_temp = tempDir - coder_dir_verde;
  coder_esq_verde = tempEsq;
  coder_dir_verde = tempDir;
  if(dif_esq_temp < 0){ dif_esq_verde += dif_esq_temp;}else{ dif_esq_verde += dif_esq_temp;}
  if(dif_dir_temp < 0){ dif_dir_verde += dif_dir_temp;}else{ dif_dir_verde += dif_dir_temp;}
  //if(dif_esq_verde < 0){ dif_esq_verde = 0;}
  //if(dif_dir_verde < 0){ dif_dir_verde = 0;}
  int dif = dif_esq_verde + dif_dir_verde;
  /*
  Serial.print(" ");
  Serial.print(dif);
  Serial.println(" ");
  */
  if(dif > 275){ //300 //150 //250
    return true;
  }
  else{
    return false;
  }
}

void encoder90Reset(){
  noInterrupts();
  coder_esq_90 = encoderEsq;
  coder_dir_90 = encoderDir;
  interrupts();
  dif_esq = 0;
  dif_dir = 0;
}

void encoderVerdeReset(){
  noInterrupts();
  coder_esq_verde = encoderEsq;
  coder_dir_verde = encoderDir;
  interrupts();
  dif_esq_verde = 0;
  dif_dir_verde = 0;
}

void curva90Esq(){
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  moveMotores(100, 100);
  delay(300);
  int angulo_inicial = angular(YAW);
  if(angular(PITCH) < 6){ // Checa se não está em uma rampa (atenção com às lombadas)
    bool perdeu_linha = false;
    while(infra(-1) > in_preto){
      moveMotores(-120, 120);
      if(difAngular(angulo_inicial, angular(YAW), MODULO) >= 100){
        perdeu_linha = true;
        break;
      }
    }
    if(perdeu_linha){
      MRUAngular(10, 150);
      moveMotores(130, 130);
      delay(200);
      Stop();
      delay(-1);
      recLinha();
    }
    else{
      //delay(100);
      //moveMotores(100, 100);
      //delay(50);
      imuVerde = millis() + 500;
    }
  }
  digitalWrite(led_esq_R, LOW);
}

void curva90Dir(){
  limpaLeds();
  digitalWrite(led_dir_R, HIGH);
  moveMotores(100, 100);
  delay(300);
  int angulo_inicial = angular(YAW);
  if(angular(PITCH) < 6){ // Checa se não está em uma rampa (atenção com às lombadas)
    bool perdeu_linha = false;
    while(infra(-1) > in_preto){
      moveMotores(120, -120);
      if(difAngular(angulo_inicial, angular(YAW), MODULO) >= 100){
        perdeu_linha = true;
        break;
      }
    }
    if(perdeu_linha){
      MRUAngular(-10, 150);
      moveMotores(130, 130);
      delay(200);
      Stop();
      delay(-1);
      recLinha();
    }
    else{
      //delay(100);
      //moveMotores(100, 100);
      //delay(50);
      imuVerde = millis() + 500;
    }
  }
  digitalWrite(led_dir_R, LOW);
}
