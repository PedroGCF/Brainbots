void procurarTriangulo(int cor);
void centralizarTriangulo(int cor);

int depositadas = EEPROM.read(64);
int vivasResgatadas = 0;
int mortasResgatadas = 0;

bool confirmacaoCor = true;

void procurarTriangulo(int cor){
  do {
    receberDados(true);
    girarDireita(115);
  } while (dado1 != cor);
  Stop();
  
  centralizarTriangulo(cor);

  long tempoMaximoAvanco = millis() + 3000;
  do {
    receberDados();
    moveMotores(100  * multiplicador, 100);
  } while (dadoA < 230 && tempoMaximoAvanco > millis());
  Stop();

  if (tempoMaximoAvanco <= millis()){
    moverTrasT(100, 600);
    procurarTriangulo(cor);
    return;
  }

  long timeoutProcurar = millis() + 10000;
  while (true){
    receberDados();
    if (timeoutProcurar <= millis()){
      procurarTriangulo(cor);
      return;
      timeoutProcurar = millis() + 10000;
    } else if (tof(FRENTEt) < 25){
      MRUAngular(180, 120);
      break;
    } else if (tof(DIRt) < 85 && dado1 == 0){
      MRUAngular(-90, 120);
      moverFrenteT(100, 700);
      procurarTriangulo(cor);
      return;
      timeoutProcurar = millis() + 10000;
    } else if (tof(ESQt) < 85 && dado1 == 0){
      MRUAngular(90, 120);
      moverFrenteT(100, 700);
      procurarTriangulo(cor);
      return;
      timeoutProcurar = millis() + 10000;
    } else{
      moverFrente(100);
    }
  }
  Stop();
  moverTrasT(120, 500);

  if (tcsCor() == 1 && cor == 4){
    ledsRed();
    vitimaMorta();
    delay(500);
    depositadas += mortasResgatadas;
    mortasResgatadas = 0;
  }else if (tcsCor() == 2 && cor == 3){
    ledsGreen();
    vitimaViva();
    delay(500);
    depositadas += vivasResgatadas;
    vivasResgatadas = 0;
  }else {
    confirmacaoCor = false;
    moverFrenteT(120, 500);
    procurarTriangulo(cor);
    return;
  }

  if (confirmacaoCor){
    for (int i = 0; i < 3; i++) {
      moverFrenteT(100, 150);
      moverTrasT(150, 300);
    }
    EEPROM.write(64, depositadas);
        
    vitimaNeutro();
    if (cor == 3){
      moverFrenteT(100, 500);
    }
  }
  limpaLeds();
}

void centralizarTriangulo(int cor) {
  piscarBlue(2, 100);

  do {
    receberDados();
    girarDireita(115);
  } while (dado1 == cor);
  Stop();
  long tempCT = millis();
  
  do {
    receberDados();
    girarEsquerda(115);
  } while (dado1 != cor);
  Stop();

  do {
    receberDados();
    girarEsquerda(115);
  } while (dado1 == cor);
  Stop();

  long tempoMCT = (millis() - tempCT) / 2;
  girarDireita(115);
  delay(tempoMCT);
  Stop();
}
