void moverFrente(int vel);
void moverFrenteT(int speed, int tempo);  // Anda para frente pelo tempo e velocidade determinado
void moverTras(int vel);
void moverTrasT(int vel, int tempo);
void stopT(int tempo);  // Fica parado pelo tempo determinado
void girarEsquerda(int vel);
void girarDireita(int vel);

void imSDR();
void angMovimentation(int sentido); // 1 - Horário e 2 - Antihorário


void imSDR(){
  Serial.print("dado1: "); 
  Serial.print(dado1);
  Serial.print(" | dadoX: "); 
  Serial.print(dadoX);
  Serial.print(" | dadoY: "); 
  Serial.print(dadoY);
  Serial.print(" | dadoA: "); 
  Serial.println(dadoA);
}

int lastAngle = 0;
int robodeveestar = 0;
int giro = 0;

void angMovimentation90(int sentido){
  Stop();
  if (sentido == 1){
    robodeveestar = ((lastAngle - 90) + 360) % 360;
  } else {
    robodeveestar = (lastAngle + 90) % 360;
  }
  
  giro = difAngular(robodeveestar, angular(YAW), MODULO);

  if (sentido == 1){
    MRUAngular(-giro + 5, 115);
  } else {
    MRUAngular(giro - 7, 115);
  }
  
  Stop();
  lastAngle = robodeveestar;
}

void angMovimentation45(int sentido){
  Stop();
  if (sentido == 1){
    robodeveestar = ((lastAngle - 45) + 360) % 360;
  } else {
    robodeveestar = (lastAngle + 45) % 360;
  }
  
  giro = difAngular(robodeveestar, angular(YAW), MODULO);

  if (sentido == 1){
    MRUAngular(-giro + 5, 115);
  } else {
    MRUAngular(giro - 5, 115);
  }
  
  Stop();
  lastAngle = robodeveestar;
}

void moverFrente(int vel) {
  moveMotores(vel, vel, 1);
}

void moverFrenteT(int speed, int tempo) {
  moveMotores(speed, speed, 1);
  delay(tempo);
  Stop();
}

void moverTras(int vel) {
  moveMotores(-vel, -vel, 1);
}

void moverTrasT(int speed, int tempo) {
  moveMotores(-speed, -speed, 1);
  delay(tempo);
  Stop();
}

void stopT(int tempo) {
  analogWrite(MesqB, 0);
  analogWrite(MesqA, 0);
  analogWrite(MdirB, 0);
  analogWrite(MdirA, 0);
  delay(tempo);
}

void girarEsquerda(int vel) {
  moveMotores(-vel, vel);
}

void girarDireita(int vel) {
  moveMotores(vel, -vel);
}
