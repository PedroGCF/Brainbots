void procurarVitimas(int graus);
void centralizarVitima();
void resgatarVitima();
void procuraExtra(String sentido, int tempo);

int tempBolinha = 0;
int resgatadas = 0;
int angInicialVitima = 0;

extern byte dado1; // Há objetos na imagem? (0 = não)
extern byte dadoX;// Cordenada X (0 - 100)
extern byte dadoY; // Cordenada Y (0 - 100)
extern byte dadoA; // Área da vítima (0 - 255)
extern byte dadoX1; // X mínimo
extern byte dadoX2; // X máximo

void procurarVitimas(int graus) { 
  while (true) {
    int actualAngleV = angular(YAW);
    int erroProcurando = difAngular(actualAngleV, angInicialVitima);
    receberDados();

    if (resgatadas == 3) {
      Stop();
      ledsMagenta();
      break;
    } 
    
    if (dado1 == 0 || dadoA >= 200) {
      if (graus > 0 && erroProcurando < graus){
        girarDireita(100);
      } else if (graus < 0 && erroProcurando > graus){
        girarEsquerda(95);
      } else{
        Stop();
        if (dado1 != 0 && dadoA < 200){
          piscarGreen(2, 50);
          centralizarVitima();
        }
        piscarRed(2, 50);
        break;
      } 
    } else {
      Stop();
      piscarGreen(2, 50);
      centralizarVitima();
    }
  }
}

void centralizarVitima() {
  int temBolinha = 0;
  int vivasDetect = 0;
  int mortasDetect = 0;

  if (dadoA <= 125){
    while (dadoA <= 125 && temBolinha <= 15) {
      receberDados();
      if (dado1 != 0) {     
        if (dadoY < 18) {girarDireita(100);} 
        else if (dadoY > 42) {girarEsquerda(100);}
        else {moverFrente(60);}

        if (dado1 == 1){mortasDetect += 1;}else{vivasDetect += 1;}
      } else{
        Stop();
        temBolinha++;
      }
    }
  } else {
    tempBolinha = 4;
  }
  Stop();
  moverTrasT(100, 150);

  if (temBolinha <= 10){
     int bolaFalsa = 0;
     tempBolinha = 0;
     while (bolaFalsa <= 15 && tempBolinha == 0){
      receberDados();
      if (dado1 != 0){tempBolinha = 3;} 
      else {bolaFalsa ++;}
    }
  }
  stopT(500); 
  
  if (tempBolinha == 3){
    if (mortasDetect * 1.2 > vivasDetect){tempBolinha = 1;} 
    else{tempBolinha = 2;}
    resgatarVitima();
  } else if (tempBolinha == 4) {
    receberDados();
    if (dado1 == 1){tempBolinha = 1;} else {tempBolinha = 2;}
    resgatarVitima();
  }
  limpaLeds();
}   

void resgatarVitima() {
  piscarBlue(2, 50);

  abreGarra();
  desceGarra();
  moverFrenteT(80, 550);
  fechaGarra();
  moverTrasT(80, 100);
  if (ultra(FRENTEu) < 10) {moverTrasT(80, 400);}
  sobeGarra();

  int sensorGarra = analogRead(A13);
  if (sensorGarra > 38){
    if (tempBolinha == 1 && mortasResgatadas == 0 && sensorGarra < 200) {
      abreGarraDir();
      abreGarra();
      resgatadas ++;
      mortasResgatadas ++;
    } else{
      abreGarraEsq();
      abreGarra();
      resgatadas ++;
      vivasResgatadas ++;
    }

    for (int i = 0; i < 3; i++) {
      moverFrenteT(100, 200);
      moverTrasT(100, 200);
    }
      
  } else {
    piscarRed(2, 50);
    abreGarraDir();
    abreGarra();
  }
}

void procuraExtra(String sentido, int tempo) {
  if (resgatadas < 3) {
    if (sentido == "Frente"){
      moverFrenteT(100, tempo);
    } else if (sentido == "Tras"){
      moverTrasT(100, tempo);
    }
    procurarVitimas(150);
    procurarVitimas(-170);
  }
}
