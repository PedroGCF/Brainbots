void rampa();
void gangorra(); // Sobe gangorras e rampas
void downgorra(); // Desce gangorras e rampas
void caidoEsq();
void caidoDir();
void verde90Rampa();
void curva90Rampa();
void verdeRampa();
void verdeEsqRampa();
void verdeEsqRampaGangorra();
void verdeEsqRampaDowngorra();
void verdeEsqRampaCaido();
void verdeDirRampa();
void verdeDirRampaGangorra();
void verdeDirRampaDowngorra();
void verdeDirRampaCaido();
void verdeDoisRampa();
void verdeDoisRampaSubida();
void verdeDoisRampaDescida();
void MRUAngularRampa(int angulo, int vel_esq, int vel_dir);
void PIDRampa();
void sLinhaLado();

void desviaGangorra();
void desviaDowngorra();

void sLinhaSubida(); // Declaração cruzada com SLinha.h
void sLinhaDescida();
void PID();

int velA = 0; // A velocidade padrão da esquerda
int velB = 0; // A velocidade padrão da direita
int velC = 0; // A velocidade de correção do verde, negativa
int velD = 0; // A velocidade de avanço extra padrão
int velE = 0; // A velocidade alternativa da esquerda(caido verde para baixo)
int velF = 0; // A velocidade alternativa da direita(caido verde para baixo)
bool pidRevertido = false;
bool ladoEsq = false;
bool ladoDir = false;
bool gangorraAtiva = false;
bool downgorraAtiva = false;
bool temVerde = false;
bool tem90 = false;
bool temLI = false;
bool rampa_ativa = false;
long giroLado = 0;
bool primeiraVez = false;
bool quebraTudo = false;

extern long imuGangorra;
extern int vel_maxima;
extern int vel_maxima_2;
extern int zona_morta;



void resetPid(){
  vel_maxima = 80; // Vel máxima frente
  vel_maxima_2 = 200; // Vel maxima frente primeiro setor
  vel_maxima_3 = 255; // Vel maxima frente segundo setor
  vel_limite = 255; // Vel máxima trás
  zona_morta = 60; // 60 para ir para frente e 100 para girar
  tempo1 = 1;
  tempo2 = 30;
}

void rampa(){
  Stop();
  delay(-1);
  
  /*
  while(true){
    digitalWrite(led_esq_B, HIGH);
    digitalWrite(led_dir_B, HIGH);
    delay(500);
    digitalWrite(led_esq_B, LOW);
    digitalWrite(led_dir_B, LOW);
    delay(500);
  }
  */
  rampa_ativa = true;
  primeiraVez = true;
  quebraTudo = false;
  bool descida = false;
  delay(1);
  //if(primeiraVez){ Serial.println("Primeira vez");} // Não toque nos imprimes, são essenciais
  while(true){
     if(angular(ROLL) < -12){
      caidoEsq();
      descida = false;
      Serial.println("Falso");
      primeiraVez = false;
    }
    else if(angular(ROLL) > 12){
      caidoDir();
      descida = false;
      Serial.println("Falso");
      primeiraVez = false;
    }
    else if(angular(PITCH) < -7){
      gangorra();
      descida = false;
      Serial.println("Falso");
      primeiraVez = false;
    }
    else if(angular(PITCH) > 7){
      bool temp_inutil2 = primeiraVez;
      delay(1);
      //Serial.println("Na rotina");
      //if(primeiraVez){ Serial.println("Na rotina e ativado");}
      downgorra();
      descida = true;
      Serial.println("Falso");
      primeiraVez = false;
      bool temp_inutil3 = primeiraVez;
      delay(1);
      //if(!primeiraVez){ Serial.println("agora zerou");}
    }
    else{ break;}
    if(quebraTudo){
      Stop();
      break;
    }
  }
  if(descida){
    moveMotores(-150, -150);
    delay(150);
  }
  else{
    //moveMotores(100, 100);
    //delay(250);
    //recLinha();
  }
  resetPid();
  rampa_ativa = false;
  if(!quebraTudo){
    imuGangorra = millis() + 3000;
    imuVerde = millis() + 250; // Para não detectar verde erroneamente
    imu90 = millis() + 2000;
  }
}

void gangorra2(){
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  pidRevertido = true;
  // /*
  vel_maxima = 255;
  vel_maxima_2 = 255;
  vel_maxima_3 = 255;
  vel_limite = -20;
  zona_morta = 60; // 20
  // */
  /*
  vel_maxima = 150;
  vel_maxima_2 = 200;
  vel_maxima_3 = 255;
  vel_limite = 100;
  zona_morta = 40;
  */
  tempo1 = 60;
  tempo2 = 20; //150
  velA = 200;
  velB = 200;
  //velC = 200;
  velD = 80;
  gangorraAtiva = true;
  while(angular(PITCH) < -6 && angular(ROLL) >= -13 && angular(ROLL) <= 13){
    //tempo_erro = 0;
    /*
    if(ultra(FRENTEu) <= 5){
      desviaGangorra();
    }
    else{
    */
      //curva90Rampa();
      curva90Rampa();
      verde90Rampa();
   // }
  }
  Stop();
  /*
  bool queda = false;
  repitaPorTempo(500){
    if(angular(PITCH) > 5){
      queda = true;
    }
  }
  if(queda){
    moveMotores(-200, -200);
    delay(150);
    Stop();
    delay(500);
    quebraTudo = true;
    imuGangorra = millis() + 3000;
  }
  */
  gangorraAtiva = false;
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_B, LOW);
}

void downgorra2(){
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  //pidRevertido = true;
  vel_maxima = 100;
  vel_maxima_2 = 140;
  vel_maxima_3 = 180;
  vel_limite = 200;
  zona_morta = 60;
  tempo1 = 15;
  tempo2 = 200;
  velA = 80;
  velB = 80;
  velC = 100;
  velD = -40;
  downgorraAtiva = true;
  Serial.println();
  Serial.println(primeiraVez);
  //if(primeiraVez){
  if(false){
    Serial.println("Primeira Vez");
    moveMotores(-150, -150);
    Serial.println(angular(PITCH));
    while(angular(PITCH) > 6){
      Serial.println(angular(PITCH));
    }
    //Stop();
    moveMotores(40, 40);
    Serial.println(angular(PITCH));
    /*
    while(true){
      Serial.print(angular(PITCH));
      Serial.print(" ");
    }
    */
    while(angular(PITCH) < 7){
      Serial.println(angular(PITCH));
    }
    Stop();
    delay(500);
    
  }
  while(angular(PITCH) > 6 && angular(ROLL) >= -13 && angular(ROLL) <= 13){
    Serial.print(angular(PITCH));
    Serial.print(" ");
    //tempo_erro = 0;
    //if(ultra(FRENTEu) <= 5){
    //  desviaDowngorra();
    //}
    //else{
      /*
      curva90Rampa();
      delay(25);
      Stop();
      delay(100)
      */
      curva90Rampa();
      verde90Rampa();
      Serial.println("Repetindo");
   // }
  }
  downgorraAtiva = false;
  pidRevertido = false;
  digitalWrite(led_esq_R, LOW);
  digitalWrite(led_dir_R, LOW);
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_B, LOW);
}

void caidoEsq(){
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_dir_B, HIGH);
  vel_maxima = 150;
  vel_maxima_2 = 200;
  vel_maxima_3 = 255;
  vel_limite = 80;
  zona_morta = 40;
  tempo1 = 60;
  tempo2 = 200;
  velA = 150;
  velB = 0;
  //velC = 150;
  velD = 60; // 100
  velE = 20;
  velF = 60;
  ladoEsq = true;
  while(angular(ROLL) < -11){
    //tempo_erro = 0;
    //curva90Rampa();
    curva90Rampa();
    verde90Rampa();
  }
  ladoEsq = false;
  if(giroLado + 1000 > millis()){
    moveMotores(100, 100);
    delay(100);
    moveMotores(-150, 0);
    delay(350);
    moveMotores(-100, -100);
    delay(250);
    Stop();
    delay(500);
  }
  digitalWrite(led_dir_R, LOW);
  digitalWrite(led_dir_B, LOW); 
}

void caidoDir(){
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_esq_B, HIGH);
  vel_maxima = 150;
  vel_maxima_2 = 200;
  vel_maxima_3 = 255;
  vel_limite = 80;
  zona_morta = 40;
  tempo1 = 60;
  tempo2 = 200;
  velA = 0;
  velB = 150;
  //velC = 150;
  velD = 60; // 100
  velE = 60;
  velF = 20;
  ladoDir = true;
  while(angular(ROLL) > 11){
    //tempo_erro = 0;
    //curva90Rampa();
    curva90Rampa();
    verde90Rampa();
  }
  ladoDir = false;
  if(giroLado + 1000 > millis()){
    moveMotores(100, 100);
    delay(100);
    moveMotores(0, -150);
    delay(350);
    moveMotores(-100, -100);
    delay(250);
    Stop();
    delay(500);
  }
  digitalWrite(led_esq_R, LOW);
  digitalWrite(led_esq_B, LOW);
}

void verde90Rampa(){
  //curva90();
  if(millis() < imuVerde){
    PIDRampa();
  }
  else{
    verdeRampa();
  }
}


void curva90Rampa(){
  //limpaLeds();
  if(reLeitura(infra, 0, in_preto, 50)){
    if(reLeitura(infra, 15, in_preto, 50)){
      imuVerde = millis() + 500;
    }
    else{ 
      imuVerde = millis() + 500;
    }
  }
  else{
    if(reLeitura(infra, 15, in_preto, 50)){
      imuVerde = millis() + 500;
    }
    else{
         
    }
  }
}
/*
void curva90Rampa(){
  tem90 = false;
  if(reLeitura(infra, 0, in_preto, 50)){
    if(reLeitura(infra, 15, in_preto, 50)){
      verde90Rampa();
      imuVerde = millis() + 250;
    }
    else{
      moveMotores(velD, velD);
      bool dois90 = false;
      int tempo_extra = 250;
      if(downgorraAtiva){ tempo_extra = 500;}
      repitaPorTempo(tempo_extra){
        bool dir90 = reLeitura(infra, 15, in_preto, 50);
        if(dir90){
          dois90 = true;
        }
      }
      Stop();
      tem90 = true;
      if(infra(6) < in_preto || infra(-1) < in_preto || infra(9) < in_preto);
      else if(dois90){
        verde90Rampa();
      }
      else{
        delay(-1);
        verdeEsqRampa();
      }
    }
    imuVerde = millis() + 250;
  }
  else{
    if(reLeitura(infra, 15, in_preto, 50)){
      moveMotores(velD, velD);
      bool dois90 = false;
      int tempo_extra = 250;
      if(downgorraAtiva){ tempo_extra = 500;}
      repitaPorTempo(tempo_extra){
        bool esq90 = reLeitura(infra, 0, in_preto, 50);
        if(esq90){
          dois90 = true;
        }
      }
      Stop();
      tem90 = true;
      if(infra(6) < in_preto || infra(-1) < in_preto || infra(9) < in_preto);
      else if(dois90){
        verde90Rampa();
      }
      else{
        delay(-1);
        verdeDirRampa();
      }
      imuVerde = millis() + 250;
    }
    else{
      verde90Rampa();
    }
  }
}
*/

void verdeRampa(){
  bool esqGreen = ehVerde(ESQ);
  bool dirGreen = ehVerde(DIR);
  temVerde = false;
  if(esqGreen){
    if(dirGreen){
      moveMotores(100, 100);
      delay(100);
      temVerde = true;
      verdeDoisRampa();
    }
    else{
      moveMotores(velD, velD);
      bool doisVerdes = false;
      repitaPorTempo(100){
        bool dirGreen = ehVerde(DIR);
        if(dirGreen){
          doisVerdes = true;
        }
      }
      Stop();
      temVerde = true;
      if(doisVerdes){
        verdeDoisRampa();
      }
      else{
        verdeEsqRampa();
      }
    }
  }
  else{
    if(dirGreen){
      moveMotores(velD, velD);
      bool doisVerdes = false;
      repitaPorTempo(100){
        bool esqGreen = ehVerde(ESQ);
        if(esqGreen){
          doisVerdes = true;
        }
      }
      Stop();
      temVerde = true;
      if(doisVerdes){
        verdeDoisRampa();
      }
      else{
        verdeDirRampa();
      }
    }
    else{
      PIDRampa();
    }
  }
  temVerde = false;
}

void verdeEsqRampa(){
  if(temVerde){
    //moveMotores(velC, velC);
    rampa_ativa = false;
    //PID();
    moveMotores(velD, velD);
    delay(500);
    rampa_ativa = true;
  }
  if(tem90){
    moveMotores(-velC, -velC);
    delay(250);
  }
  if(gangorraAtiva){
    verdeEsqRampaGangorra();
  }
  else if(downgorraAtiva){
    verdeEsqRampaDowngorra();
  }
  else{
    verdeEsqRampaCaido();
  }
}

void verdeEsqRampaGangorra(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  //delay(50);
  vel_maxima = 255;
  vel_maxima_2 = 255;
  vel_maxima_3 = 255;
  vel_limite = -20;
  zona_morta = 60; // 20
  tempo1 = 60;
  tempo2 = 20; //150
  velA = 200;
  velB = 200;
  //velC = 200;
  velD = 80;
  
  repitaPorTempo(750){
    delay(5);
    PID();
  }
  Stop();
  delay(1000);
  MRUAngularRampa(-35, -20, 255);
  moveMotores(-60, 0);
  delay(50);
  Stop();
  delay(1000);
  moveMotores(0, 120);
  while(!reLeitura(infra, -1, in_preto, 200));
  moveMotores(0, -200);
  delay(25);
  //moveMotores(-80, -80);
  //delay(250);
  Stop();
  delay(1000);
  digitalWrite(led_esq_G, LOW);
}

void verdeEsqRampaDowngorra(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  Stop();
  delay(500);
  //moveMotores(-200, -200);
  //delay(250);
  MRUAngularRampa(-20, 255, -20);
  Stop();
  delay(500);
  MRUAngularRampa(-40, 0, 60);
  moveMotores(0, 100);
  while(!reLeitura(infra, -1, in_preto, 200));
  Stop();
  delay(500);
  digitalWrite(led_esq_G, LOW);
}

void verdeEsqRampaCaido(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  Stop();
  delay(1000);
  if(ladoEsq){
    MRUAngularRampa(-25, velE, velF * 1.3);
    Stop();
    delay(1000);
    MRUAngularRampa(-25, velA, -velE);
    moveMotores(-velA, -velE);
    while(!reLeitura(infra, -1, in_preto, 200));
    moveMotores(velA, -velE);
    delay(100);
    moveMotores(-velA, -velE);
    delay(100);
    Stop();
    delay(1000);
    MRUAngularRampa(-25, velE, velF);
    moveMotores(-velE, velF);
    while(!reLeitura(infra, 7, in_preto, 200));
    while(angular(ROLL) < -3);
    //moveMotores(-200, -200);
    //delay(250);
    moveMotores(-20, -60);
    delay(500);
  }
  else{
    MRUAngularRampa(-35, velA, velB);
    moveMotores(-velA, velB);
    while(!reLeitura(infra, -1, in_preto, 200));
  }
  Stop();
  delay(500);
  quebraTudo = true;
  digitalWrite(led_esq_G, LOW);
}

void verdeDirRampa(){
  if(temVerde){
    //moveMotores(velC, velC);
    rampa_ativa = false;
    //PID();
    moveMotores(velD, velD);
    delay(500);
    rampa_ativa = true;
  }
  if(tem90){
    moveMotores(-velC, -velC);
    delay(250);
  }
  if(gangorraAtiva){
    verdeDirRampaGangorra();
  }
  else if(downgorraAtiva){
    verdeDirRampaDowngorra();
  }
  else{
    verdeDirRampaCaido();
  }
}

void verdeDirRampaGangorra(){
  limpaLeds();
  digitalWrite(led_dir_G, HIGH);
  //delay(50);
  vel_maxima = 255;
  vel_maxima_2 = 255;
  vel_maxima_3 = 255;
  vel_limite = -20;
  zona_morta = 60; // 20
  tempo1 = 60;
  tempo2 = 20; //150
  velA = 200;
  velB = 200;
  //velC = 200;
  velD = 80;
  
  repitaPorTempo(750){
    delay(5);
    PID();
  }
  Stop();
  delay(1000);
  MRUAngularRampa(35, 255, -20);
  moveMotores(0, -60);
  delay(50);
  Stop();
  delay(1000);
  moveMotores(120, 0);
  while(!reLeitura(infra, -1, in_preto, 200));
  moveMotores(-200, 0);
  delay(25);
  //moveMotores(-80, -80);
  //delay(250);
  Stop();
  delay(1000);
  digitalWrite(led_dir_G, LOW);
}

void verdeDirRampaDowngorra(){
  limpaLeds();
  digitalWrite(led_dir_G, HIGH);
  Stop();
  delay(500);
  //moveMotores(-200, -200);
  //delay(250);
  MRUAngularRampa(20, -20, 255);
  Stop();
  delay(500);
  MRUAngularRampa(40, 60, 0);
  moveMotores(100, 0);
  while(!reLeitura(infra, -1, in_preto, 200));
  Stop();
  delay(500);
  digitalWrite(led_dir_G, LOW);
}

void verdeDirRampaCaido(){
  limpaLeds();
  digitalWrite(led_dir_G, HIGH);
  Stop();
  delay(1000);
  if(ladoDir){
    MRUAngularRampa(25, velE * 1.3, velF); // // //
    Stop();
    delay(1000);
    MRUAngularRampa(25, -velF, velB);
    moveMotores(-velF, -velB);
    while(!reLeitura(infra, -1, in_preto, 200));
    moveMotores(-velF, velB);
    delay(75); // // //
    moveMotores(-velF, -velB);
    delay(100);
    Stop();
    delay(1000);
    MRUAngularRampa(25, velE, velF);
    moveMotores(velE, -velF);
    while(!reLeitura(infra, 8, in_preto, 200));
    while(angular(ROLL) > 3);
    //moveMotores(-200, -200);
    //delay(250);
    moveMotores(-60, -20);
    delay(500);
  }
  else{
    MRUAngularRampa(35, velA, velB);
    moveMotores(velA, -velB);
    while(!reLeitura(infra, -1, in_preto, 200));
  }
  Stop();
  delay(500);
  quebraTudo = true;
  digitalWrite(led_dir_G, LOW);
}

void verdeDoisRampa(){
  if(angular(PITCH) < 0){
    verdeDoisRampaSubida();
  }
  else{
    verdeDoisRampaDescida();
  }
}

void verdeDoisRampaSubida(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
  int temp = angular(YAW);
  moveMotores(velA, velB);
  delay(750);
  Stop();
  delay(1000);
  MRUAngularRampa(-80, 60, 0);
  delay(500);
  moveMotores(150, 150);
  delay(200);
  MRUAngularRampa(-60, 150, 60);
  delay(500);
  moveMotores(-150, 0);
  while(difAngular(temp, angular(YAW), MODULO) < 175);
  //while(reLeitura(infra, -1, in_preto, 200, true));'
  Stop();
  delay(500);
  moveMotores(-150, -150);
  delay(500);
  Stop();
  delay(1000);
  digitalWrite(led_esq_G, LOW);
  digitalWrite(led_dir_G, LOW);
}

void verdeDoisRampaDescida(){
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
  Stop();
  delay(500);
  moveMotores(velA, velB);
  delay(250);
  MRUAngularRampa(-90, 0, 150);
  MRUAngularRampa(-60, 150, 0);
  moveMotores(0, velB);
  while(!reLeitura(infra, -1, in_preto, 200));
  moveMotores(velA, velB);
  delay(250);
  Stop();
  delay(500);
  digitalWrite(led_esq_G, LOW);
  digitalWrite(led_dir_G, LOW);
}

void MRUAngularRampa(int angulo, int vel_esq, int vel_dir){
  bno.getEvent(&event);
  int angulo_atual = event.orientation.x;
  int angulo_desejado = (angulo_atual + angulo) % 360; // Corrige o loop do 360/0
  if (angulo_desejado < 0) angulo_desejado += 360; // Ajusta ângulos negativos

  if (angulo > 0) {
    moveMotores(vel_esq, -vel_dir); // Positivo → sentido horário
  } else {
    moveMotores(-vel_esq, vel_dir); // Negativo → sentido antihorário
  }

  int erro = 5;
  do { // Aguarda atingir o ângulo desejado, considerando margem de erro
    bno.getEvent(&event);
    angulo_atual = event.orientation.x;
  } while (!(angulo_atual >= angulo_desejado - erro && angulo_atual <= angulo_desejado + erro));

  Stop();
}

void PIDRampa(){
  if(ladoEsq || ladoDir){
    sLinhaLado();
    //PID();
  }
  else{
    PID();
  }
}

void sLinhaLado(){
  if(ladoEsq){
    if(infra(9) < in_preto){
      moveMotores(100, 20);
    }
    else if(infra(6) < in_preto){
      moveMotores(0, 130);
      delay(30);
      moveMotores(0, 20);
      delay(250);
      giroLado = millis();
    }
    else{
      moveMotores(60, 60);
    }
  }
  else if(ladoDir){
    if(infra(6) < in_preto){
      moveMotores(20, 100);
    }
    else if(infra(9) < in_preto){
      moveMotores(130, 0);
      delay(30);
      moveMotores(20, 0);
      delay(250);
      giroLado = millis();
    }
    else{
      moveMotores(60, 60);
    }
  }
}






void desviaGangorra(){
  temLI = false;
  Stop();
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  delay(500);

  if ((ultra(FRENTEu) < 4)){
    while (ultra(FRENTEu) < 4){
      moveMotores(-60, -60);
    }
    Stop();
  }

  int angDG = angular(YAW);
  int difDG = 0;
  do {
    moveMotores(0, 120);
    difDG = difAngular(angular(YAW), angDG, MODULO);
  } while (difDG < 70);
  
  Stop();
  delay(500);

  if (tof(DIRt) > 100){
    while(tof(DIRt) > 100){
      moveMotores(-80, -80);
    }
  }
  Stop();

  moveMotores(80, 80);
  delay(200);
  Stop();

  angDG = angular(YAW);
    difDG = 0;
    do {
      moveMotores(120, 0);
      difDG = difAngular(angular(YAW), angDG, MODULO);
    } while (difDG < 80);
    Stop();
    delay(500);

  for (int i = 0; i < 3; i++) {
    limpaLeds();
    while(infra(15) > in_preto && tof(DIRt) > 100){
       moveMotores(80, 80);
    }
    if (infra(15) < in_preto){
      moveMotores(80, 80);
      delay(300);
      moveMotores(0, 150);
      while(!reLeitura(infra, -1, in_preto, 200));
    } else{
      while(infra(15) > in_preto && tof(DIRt) < 100){
        moveMotores(80, 80);
      }
      Stop();
      if (infra(15) < in_preto){
        digitalWrite(led_esq_R, HIGH);
        digitalWrite(led_dir_R, HIGH);
        moveMotores(80, 80);
        delay(300);
        Stop();
        while(infra(15) > in_preto){
          moveMotores(0, 150);
        }
        Stop();
        break;
      } else{
        digitalWrite(led_esq_G, HIGH);
        digitalWrite(led_dir_G, HIGH);
        moveMotores(80,80);
        delay(1500);
        Stop();
        delay(500);
        angDG = angular(YAW);
        difDG = 0;
        do { 
          moveMotores(120, 0);
          if (infra(15) < in_preto){
            temLI = true;
          }
          difDG = difAngular(angular(YAW), angDG, MODULO);
        } while (difDG < 85);
        if (temLI){
          Stop();
          while(infra(15) > in_preto){
            moveMotores(-80, -80);
          }
          Stop();
          delay(500);
          angDG = angular(YAW);
          do { 
            moveMotores(-120, 0);
            difDG = difAngular(angular(YAW), angDG, MODULO);
          } while (difDG < 30);
          Stop();
          delay(500);
          while(infra(15) > in_preto){
            moveMotores(80, 80);
          }
          moveMotores(80, 80);
          delay(350);
          while(infra(15) > in_preto){
            moveMotores(0, 120);
          }
          moveMotores(-80,-80);
          delay(200);
          break;
        }
        Stop();
        delay(500);
      }
    }
    limpaLeds();
  }
}



void desviaDowngorra(){
  temLI = false;
  Stop();
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  delay(500);

  while (ultra(FRENTEu) < 5){
    moveMotores(-100, -100);
  }
  Stop();

  int angDG = angular(YAW);
  int difDG = 0;
  moveMotores(-100, -100);
  delay(700);
  Stop();
  do {
    moveMotores(80, 0);
    difDG = difAngular(angular(YAW), angDG, MODULO);
  } while (difDG < 70);
  
  Stop();
  delay(500);

  while(tof(ESQt) > 100){
    moveMotores(-80, -80);
  }
  Stop();
  delay(500);

  moveMotores(80, 80);
  delay(150);
  Stop();
  delay(500);

  angDG = angular(YAW);
    difDG = 0;
    do {
      moveMotores(-20, 80);
      difDG = difAngular(angular(YAW), angDG, MODULO);
    } while (difDG < 50);
    Stop();
    delay(500);
    do {
      moveMotores(-20, 40);
      difDG = difAngular(angular(YAW), angDG, MODULO);
    } while (difDG < 75);
    Stop();
    delay(-1);

  for (int i = 0; i < 3; i++) {
    limpaLeds();
    while(infra(0) > in_preto && tof(ESQt) > 100){
       moveMotores(80, 80);
    }
    if (infra(0) < in_preto){
      moveMotores(80, 80);
      delay(300);
      moveMotores(60, 0);
      while(!reLeitura(infra, -1, in_preto, 200));
    } else{
      while(infra(0) > in_preto && tof(ESQt) < 100){
        moveMotores(80, 80);
      }
      Stop();
      if (infra(0) < in_preto){
        digitalWrite(led_esq_R, HIGH);
        digitalWrite(led_dir_R, HIGH);
        moveMotores(80, 80);
        delay(300);
        Stop();
        while(infra(0) > in_preto){
          moveMotores(60, 0);
        }
        Stop();
        break;
      } else{
        digitalWrite(led_esq_G, HIGH);
        digitalWrite(led_dir_G, HIGH);
        moveMotores(80,80);
        delay(1500);
        Stop();
        delay(500);
        angDG = angular(YAW);
        difDG = 0;
        do { 
          moveMotores(0, 60);
          if (infra(0) < in_preto){
            temLI = true;
          }
          difDG = difAngular(angular(YAW), angDG, MODULO);
        } while (difDG < 85);
        if (temLI){
          Stop();
          while(infra(0) > in_preto){
            moveMotores(-80, -80);
          }
          Stop();
          delay(500);
          angDG = angular(YAW);
          do { 
            moveMotores(0, -60);
            difDG = difAngular(angular(YAW), angDG, MODULO);
          } while (difDG < 30);
          Stop();
          delay(500);
          while(infra(0) > in_preto){
            moveMotores(80, 80);
          }
          moveMotores(80, 80);
          delay(350);
          while(infra(0) > in_preto){
            moveMotores(60, 0);
          }
          moveMotores(-80,-80);
          delay(200);
          break;
        }
        Stop();
        delay(500);
      }
    }
    limpaLeds();
  }
}
