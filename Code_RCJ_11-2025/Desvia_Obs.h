void desviaEsq();
void desviaDir();
void acharEsq();
void acharDir();
void resetPid();
void angMove(int sentidoO);
void sLinha();

void moverFrenteT(int speed, int tempo);  // Anda para frente pelo tempo e velocidade determinado
void moverTras(int vel);
void moverTrasT(int vel, int tempo);

int lastAngleO = 0;
int robodeveestarO = 0;
int giroO = 0;

void sLinhaO(){
  if(sCor(ESQ, LUZ) > apds_preto_esq){
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(60, 60);
      if(infra(-1) > in_preto){
        nLinha = true;
      }
    }
    else{
      moveMotores(-120, 120);
    }
  }
  else{
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(120, -120);
    }
    else{
      moveMotores(60, 60);
    }
  }
}

void angMove(int sentidoO){
  Stop();
  if (sentidoO == 1){
    robodeveestarO = ((lastAngleO - 90) + 360) % 360;
  } else {
    robodeveestarO = (lastAngleO + 90) % 360;
  }
  
  giroO = difAngular(robodeveestarO, angular(YAW), MODULO);

  if (sentidoO == 1){
    MRUAngular(-giroO + 5, 120);
  } else {
    MRUAngular(giroO - 5, 120);
  }
  
  Stop();
  lastAngleO = robodeveestarO;
}

void acharEsq(){
  Stop();
  delay(250);
  //moverTrasT(60, 250);
  moveMotores(-60, -60);
  while(!reLeitura(infra, 15, in_preto, 10) && !reLeitura(infra, 14, in_preto, 10) && !reLeitura(infra, 13, in_preto, 10));
  Stop();
  delay(250);
  moveMotores(60, 60);
  delay(450);
  //angMove(2);
  moveMotores(100, -100);
  while (ultra(FRENTEu) > 10);
  delay(350);
  Stop();
  delay(250);
  moveMotores(-60, -60);
  delay(150);
  while (ultra(FRENTEu) > 4){
    sLinhaO();
  }
  Stop();
  angMove(2);
  angMove(2);
  angMove(2);
  Stop();

  moveMotores(-60, -60);
  delay(500);
  Stop();
  delay(300);
}

void desviaEsq(){
  limpaLeds();
  
  digitalWrite(led_esq_B, HIGH);
  Stop();
  delay(500);

  if ((ultra(FRENTEu) > 8)){
    while (ultra(FRENTEu) > 3.5){
      moveMotores(60, 60);
    }
    Stop();
  }
  
  if ((ultra(FRENTEu) < 3)){
    while (ultra(FRENTEu) < 3){
      moveMotores(-60, -60);
    }
    Stop();
  }
  while (ultra(FRENTEu) < 10){
     moveMotores(100, -100);
  }
  float tempoInicial = millis();
  moveMotores(-100, 100);
  delay(250);
  while(ultra(FRENTEu) < 10){
    moveMotores(-100, 100);
  }
  float tempoFinal = millis();
  float tempoGiroObs = (tempoFinal - tempoInicial) / 2;
  moveMotores(100, -100);
  delay(tempoGiroObs);
  Stop();  
  delay(250);
  lastAngleO = angular(YAW);
  
  angMove(1);
  long temp = millis() + 500;
  while(tof(DIRt) > 150 &&  millis() < temp){
    moveMotores(100, 100);
  }
  temp = millis() + 500;
  while(tof(DIRt) <= 150 && millis() < temp ){
     moveMotores(100, 100);
  }
  Stop();
  delay(500);
  temp = millis() + 500;
  while(tof(DIRt) > 150 && millis() < temp){
    moveMotores(-60, -60);
  }
  moveMotores(20, 20);
  delay(250);
  moveMotores(60, 60);
  delay(650);
  angMove(2);
  Stop();
  delay(250);
  
  bool linha = false;
  for (int i = 0; i < 3; i++){
    moveMotoresEncoder(80, 80);
    delay(150);
    while(tof(DIRt) > 140){
      repitaPorTempo(100){
        if (i != 0){
          if(reLeitura(infra, 15, in_preto, 10) || reLeitura(infra, 14, in_preto, 10) || reLeitura(infra, 13, in_preto, 10)){ linha = true; break;} 
        }
      }
    }
    if(linha){break;}
    Stop();
    moveMotoresEncoder(80, 80);
    while(tof(DIRt) <= 160){
      repitaPorTempo(100){
        if (i != 1){
          if(reLeitura(infra, 15, in_preto, 10) || reLeitura(infra, 14, in_preto, 10) || reLeitura(infra, 13, in_preto, 10)){ linha = true; break;}
        }
      }
    }
    if(linha){ break;}
    Stop();
    delay(650);
    moveMotores(60, 60);
    delay(250);
    angMove(2);
  }
  acharEsq();
}



void acharDir(){
  Stop();
  delay(250);
  //moverTrasT(60, 250);
  moveMotores(-60, -60);
  while(!reLeitura(infra, 0, in_preto, 10) && !reLeitura(infra, 1, in_preto, 10) && !reLeitura(infra, 2, in_preto, 10));
  Stop();
  delay(250);
  moveMotores(60, 60);
  delay(450);
  //angMove(1);
  moveMotores(-120, 120);
  while (ultra(FRENTEu) > 10);
  delay(350);
  Stop();
  delay(250);
  moveMotores(-60, -60);
  delay(150);
  while (ultra(FRENTEu) > 4){
    sLinhaO();
  }
  Stop();
  angMove(1);
  angMove(1);
  angMove(1);
  Stop();

  moveMotores(-60, -60);
  delay(500);
  Stop();
  delay(300);
}

void desviaDir(){
  limpaLeds();
  
  digitalWrite(led_dir_B, HIGH);
  Stop();
  delay(500);
 
  if ((tof(FRENTEt) <= 50)){
    while (tof(FRENTEt) <= 50){
      moveMotores(-60, -60);
    }
    Stop();
  }
  while ((tof(FRENTEt) < 100)){
     moveMotores(100, -100);
  }
  float tempoInicial = millis();
  moveMotores(-100, 100);
  delay(250);
  while((tof(FRENTEt) < 100)){
    moveMotores(-100, 100);
  }
  float tempoFinal = millis();
  float tempoGiroObs = (tempoFinal - tempoInicial) / 2;
  moveMotores(100, -100);
  delay(tempoGiroObs);
  Stop();  
  delay(250);
  lastAngleO = angular(YAW);
  
  angMove(2);
  long temp = millis() + 500;
  while(tof(ESQt) > 150 &&  millis() < temp){
    moveMotores(80, 80);
  }
  temp = millis() + 500;
  while(tof(ESQt) <= 150 && millis() < temp ){
     moveMotores(80, 80);
  }
  Stop();
  delay(500);
  temp = millis() + 500;
  while(tof(ESQt) > 150 && millis() < temp){
    moveMotores(-60, -60);
  }
  Stop();
  moveMotores(60, 60);
  delay(600);
  angMove(1);
  Stop();
  delay(250);

  bool linha = false;
  int sensivel = 5;
  
  for (int i = 0; i < 3; i++){
    moveMotoresEncoder(80, 80);
    delay(150);
    Stop();
    while(tof(ESQt) > 140){
      moveMotoresEncoder(70, 70);
      if(reLeitura(infra, 0,in_preto + sensivel, 25) || reLeitura(infra, 8,in_preto + sensivel, 25) || reLeitura(infra, 2,in_preto + sensivel, 25)){
        linha = true; 
        break;
      }
    }
    Stop();
    while(tof(ESQt) < 160){
      moveMotoresEncoder(70, 70);
      if(reLeitura(infra, 0,in_preto + sensivel, 25) || reLeitura(infra, 1,in_preto + sensivel, 25) || reLeitura(infra, 2,in_preto + sensivel, 25)){ 
        linha = true; 
        break;
      }
    }
    if (linha){
      break;
    }
    Stop();
    moveMotores(60, 60);
    delay(300);
    Stop();
    angMove(1);
  }
  acharDir();
}
