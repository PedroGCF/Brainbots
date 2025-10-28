void setupResgate();
void setupTriangulo();

void setupResgate() {
  piscarWhite(2, 50);
  envioOn('A');

  moverFrenteT(100, 600);
  angInicialVitima = angular(YAW);  // Setar ângulo relativo inicial da sala de resgate
}

void setupTriangulo() {
  limpaLeds();
  envioOn('B');
}
