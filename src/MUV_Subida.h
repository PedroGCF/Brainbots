void gangorra(); // Sobe gangorras e rampas
void downgorra(); // Desce gangorras e rampas

void sLinhaSubida(); // Declaração cruzada com SLinha.h
void sLinhaDescida();
void PID();

extern long imuGangorra;



void gangorra(){
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  long tempoSubida = millis();
  //comLimite = true;
  //gangorraAtiva = true;
  pidRevertido = true;
  vel_maxima = 180;
  vel_maxima_2 = 200;
  vel_maxima_3 = 255;
  zona_morta = 40;
  tempo2 = 0;
  repitaPorTempo(150){PID();}
  Stop();
  delay(250);
  if(angular(ROLL) > 6){ // setpoint: 0 // -3
    moveMotores(0, 120);
    //while(angular(ROLL) > 0);
    while(!reLeitura(infra, -1, in_preto, 50) && angular(ROLL) > 0);
  }
  else if(angular(ROLL) < -6){
    moveMotores(120, 0);
    //while(angular(ROLL) < -6);
    while(!reLeitura(infra, -1, in_preto, 50) && angular(ROLL) < 0);
  }
  Stop();
  delay(500);
  int vel_maxima_temp = vel_maxima;
  while(angular(PITCH) < -7){ // -7
    vel_maxima = vel_maxima_2 = vel_maxima_temp + round((millis() - tempoSubida) / 150);
    if (vel_maxima > 255){ vel_maxima = vel_maxima_2 = 255;}
    PID();
  }
  //comLimite = false;
  //gangorraAtiva = false;
  pidRevertido = false;
  bool queda = false;
  resetPid();
  Stop();
  //long desaceleração = millis();
  repitaPorTempo(500){
    //moveMotores(100 - round(desaceleração / 5), 100  - round(desaceleração / 5));

    //moveMotores(-60, -60);
    //delay(50);
    //moveMotores(20, 20);
    //delay(50);
    
    if(angular(PITCH) > 5){
      queda = true;
    }
  }
  if(queda){
    moveMotores(-200, -200);
    delay(250);
    Stop();
    delay(500);
    imuGangorra = millis() + 2000;
    tempoComLimite = millis() + 500;
  }
  else{
    moveMotores(-60, -60);
    delay(600);
    //imuGangorra = millis() + 1000; // Uma imunidade bem mais curta [como se 4000ms já tivessem passado na comparação]
  }
  //imuVerde = millis() + 250; // Para não detectar verde erroneamente
  imuGangorra = millis()+ 3000;
  tempoComLimite = millis() + 500;
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_B, LOW);
}

void downgorra(){
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  vel_maxima = 120;
  vel_maxima_2 = 140;
  vel_maxima_3 = 160;
  vel_limite = 200;
  zona_morta = 60;
  tempo1 = 1;
  tempo2 = 10;
  //moveMotores(-60, -60);
  //delay(1000);
  Stop();
  delay(1000);
  long tempEncoderTotal = encoderEsq + encoderDir;
  int contagem = 0;
  while(angular(PITCH) > 9){ // 7
    repitaPorTempo(100){
      PID();
    }
    noInterrupts();
    long encoderTotal = encoderEsq + encoderDir;
    interrupts();
    if(encoderTotal <= tempEncoderTotal + 5){
      contagem = contagem + 1;
    }
    tempEncoderTotal = encoderTotal;
    if(contagem == 30){
      moveMotores(255, 255);
      while(angular(PITCH) > 9);
    }   
  }
  moveMotores(-100, -100);
  delay(750);
  resetPid();
  Stop();
  imuGangorra = millis()+ 3000;
  imuVerde = millis() + 250;
  tempoComLimite = millis() + 500;
  digitalWrite(led_esq_R, LOW);
  digitalWrite(led_dir_R, LOW);
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_B, LOW);
}

/*
void gangorra(){
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  while(angular(PITCH) < -7){
    sLinhaSubida();
  }
  Stop();
  delay(500);
  if(angular(PITCH) > 4){
    moveMotores(-200, -200);
    delay(500);
    Stop();
    delay(500);
    imuGangorra = millis() + 3000;
  }
  else{
    imuGangorra = millis() + 1000; // Uma imunidade bem mais curta [como se 4000ms já tivessem passado na comparação]
  }
  imuVerde = millis() + 250; // Para não detectar verde erroneamente
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_B, LOW);
}

void downgorra(){
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  delay(200);
  Stop();
  delay(1000);
  while(angular(PITCH) > 7){
    sLinhaDescida();
  }
  moveMotores(-200, -200);
  delay(800);
  Stop();
  imuGangorra = millis()+ 3000;
  imuVerde = millis() + 250;
  digitalWrite(led_esq_R, LOW);
  digitalWrite(led_dir_R, LOW);
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_B, LOW);
}
*/
