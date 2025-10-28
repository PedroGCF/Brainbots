void procurarSaida();
void conferirSaidaL();
void conferirFrente();
void sairPreto();
void desviarPrata(int caso);

int devoSair = EEPROM.read(65);

int distanciaTOFD = 350;
int distanciaTF = 120;
int distanciaUF = 0;
int distanciaTOFM = 50;

int tolerancia = 4;
int anguloAtual;
int diferenca;
int angReferencia = 0;

float multiplicador = 1.053;

long imunidadeDP = 0;
long imunidadeSI = 0;

bool temT = false;
bool acheiSaida = false;

void procurarSaida() {
  int tofF = 0;
  int infM = 0;
  int tofD = 0;
  int praP = 0;
  
  envioOn('B');
  
  if (!devoSair){
    int angTAnt = (angular(YAW) + 45) % 360;
    lastAngle = angular(YAW);
    moverFrenteT(80, 100);
    angMovimentation90(2);
    stopT(200);
    MRUAngular(12, 120);
    if (tof(FRENTEt) < 320){
      while (tof(FRENTEt) > 175){
        moveMotores(70  * multiplicador, 70);
      }
    } else {
      moverFrenteT(80, 600);
    }
    Stop();
    lastAngle = angular(YAW);
    angMovimentation45(1);
    stopT(300);
    MRUAngular(-5, 120);
    Stop();
    lastAngle = angTAnt;
  } else {
    lastAngle = angular(YAW);
    moverFrenteT(80, 1000);
    Stop();
    if (tof(DIRt) > 150){
      moverTrasT(80, 400);
      angMovimentation90(2);
      stopT(200);
      MRUAngular(15, 120);
      Stop();
    } else {
      moverTrasT(80, 350);
    }
    imunidadeSI = millis() + 800;
    while (millis() < imunidadeSI && tof(DIRt) > distanciaTOFD && tof(FRENTEt) > distanciaTF + 20){
      moverFrente(100);   
    }
    Stop();
    imunidadeDP = millis() + 500;    
  }
  lastAngle = angular(YAW);
  
  receberDados();
  temT = (dado1 != 0 && dadoX != 0);

  devoSair = 1;
  EEPROM.write(65, devoSair);

  long tempProcurando = millis();
  while (true) {
    moveMotores(60  * multiplicador, 60);
    tempProcurando = millis() + 7000;
    receberDados();
    temT = (dado1 != 0 && dadoX != 0);
    do {
      tofF = tof(FRENTEt);
      infM = infra(-1);
      tofD = tof(DIRt);
      praP = analogRead(prata_pin);
    } while (tofD < distanciaTOFD && tofF > distanciaTF && praP < prata && praP > 120 && millis() < tempProcurando && tofD > distanciaTOFM);
    
    receberDados();
    temT = (dado1 != 0 && dadoX != 0);

    if (millis() >= tempProcurando){
      moverTrasT(80, 500);
      MRUAngular(-15, 90);
    } else if (tofF < distanciaTF) {
      conferirFrente();
    } else if (tofD > distanciaTOFD && imunidadeDP < millis()) {
      conferirSaidaL();
      if (acheiSaida){
        break;
      }
    } else if (praP > prata && !temT) {
      desviarPrata(1);
    } else if (praP < 120 && !temT) {
      sairPreto();
      break;
    } else if (tofD < distanciaTOFM) {
      stopT(100);
      MRUAngular(-8, 100);
    }
  }
}

void conferirSaidaL() {
  acheiSaida = false;
  Stop();
  moverFrenteT(100, 155);
  angMovimentation90(2);
  stopT(200);
  MRUAngular(3, 120);
  Stop();

  do {
    moverFrente(65);
  } while (!reLeitura(infra, 7, in_preto, 25) && analogRead(prata_pin) > 120 && analogRead(prata_pin) < prata);
;
  Stop();
  if (analogRead(prata_pin) > prata) {
    desviarPrata(2);
  } else {
    sairPreto();
    acheiSaida = true;
  }
}

void conferirFrente() {
  receberDados();
  temT = (dado1 != 0 && dadoX != 0);
  
  if (temT) {
    int angAntes = lastAngle;
    angMovimentation45(1);
    MRUAngular(-3, 100);
    
    moverFrenteT(100, 350);
    angMovimentation90(1);
    while (digitalRead(toque_esq) != 0) {
      moverTras(120);
    }
    moverTrasT(120, 500);
    lastAngle = angular(YAW);
    angMovimentation90(2);
    stopT(200);
    MRUAngular(9, 120);
    if (tof(FRENTEt) < 320){
      while (tof(FRENTEt) > 175){
        moveMotores(70  * multiplicador, 70);
      }
    } else {
      moverFrenteT(80, 600);
    }
    Stop();
    limpaLeds();
    angMovimentation45(1);
    MRUAngular(-5, 120);
    lastAngle = ((angAntes - 90) + 360) % 360;
  } else {
    if (tof(DIRt) < distanciaTOFD) {
      angMovimentation90(1);
      int tempCF = millis() + 500;
      while (digitalRead(toque_esq) != 0 && millis() < tempCF) {
        moverTras(120);
      }
      moverTrasT(120, 500);
    } else {
      angMovimentation90(1);
    }
  }
  receberDados();
  temT = (dado1 != 0 && dadoX != 0);
}


void sairPreto() {
  Stop();
  piscarRed(2, 50);
}

void desviarPrata(int caso) {
  bool alinharDP = false;
  Stop();
  if (tof(DIRt) < distanciaTOFD){
    piscarWhite(2, 50);
    moverTrasT(80, 500);
    angMovimentation90(1);
    lastAngle = angular(YAW);
    if (caso == 1){
      int tempCF = millis() + 500;
      while (digitalRead(toque_esq) != 0 && millis() < tempCF) {
        moverTras(100);
      }
      moverTrasT(100, 500);
    }
    
  } else {
    Stop();
    piscarWhite(2, 50);
    moverTrasT(80, 300);
    angMovimentation90(1);
    stopT(200);
    MRUAngular(-9, 120);
    Stop();
  }
  
  Stop();
  receberDados();
  temT = (dado1 != 0 && dadoX != 0);

  imunidadeDP = millis() + 1500;
}
