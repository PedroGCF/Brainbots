// repitaPorTempo(millis){}
float media(float (*lerSensor)(), int vezes); // Dá a média de N leituras de um sensor (Sensor ou função[sem os ()], numero de leituras)
float media(float (*lerSensor)(int), int argumento, int vezes); // A exata mesma coisa, só que o argumento intermediário é o parâmetro que você usaria para a leitura [Acho que só pode int ou enum]
float media(int (*lerSensor)(int), int argumento, int vezes); // Idem mas só que int
float mediana(int (*lerSensor)(int), int argumento, int vezes); // Idem segunda média
bool reLeitura(int (*leituraSensor)(int), int argumento, float comparador, int quantidade); // (Função, Argumento, Variável [> Leitura], n° leituras)
bool reLeitura(float (*leituraSensor)(int), int argumento, float comparador, int quantidade); // Idem só que para float



#define _REPITA_POR_TEMPO_UNICO(nome, ms) \
  for (unsigned long nome = millis() + (ms); millis() < nome; ) // Complemento à segunda
#define repitaPorTempo(ms) \
  _REPITA_POR_TEMPO_UNICO(_tempo_limite_##__COUNTER__, ms)

float media(float (*lerSensor)(), int vezes) {
  float soma = 0;
  for (int i = 0; i < vezes; i++) {
    soma += lerSensor();
  }
  return soma / vezes;
}

float media(float (*lerSensor)(int), int argumento, int vezes) {
  float soma = 0;
  for (int i = 0; i < vezes; i++) {
    soma += lerSensor(argumento);
  }
  return soma / vezes;
}

float media(int (*lerSensor)(int), int argumento, int vezes) {
  float soma = 0;
  for (int i = 0; i < vezes; i++) {
    soma += lerSensor(argumento);
  }
  return soma / vezes;
}

float mediana(int (*lerSensor)(int), int argumento, int vezes) { // O calculo da média não é de minha autoria
  float valores[100]; // Tamanho Máximo
  for (int i = 0; i < vezes; i++) {
    valores[i] = lerSensor(argumento);
  }
  for (int i = 0; i < vezes - 1; i++) {
    for (int j = 0; j < vezes - i - 1; j++) {
      if (valores[j] > valores[j + 1]) {
        float temp = valores[j];
        valores[j] = valores[j + 1];
        valores[j + 1] = temp;
      }
    }
  }
  if (vezes % 2 == 0) {
    return (valores[vezes/2 - 1] + valores[vezes/2]) / 2;
  } 
  else {
    return valores[vezes/2];
  }
}

bool reLeitura(int (*leituraSensor)(int), int argumento, float comparador, int quantidade) {
  int leiturasConsecutivas = 0;
  while (leiturasConsecutivas < quantidade) {
    float leitura = leituraSensor(argumento); // Faz a leitura com argumento
    if (leitura < comparador) {
      leiturasConsecutivas++;
    } else {
      return false; // Leitura inválida
    }
  }
  return true; // Todas as leituras foram válidas
}

bool reLeitura(float (*leituraSensor)(int), int argumento, float comparador, int quantidade) {
  int leiturasConsecutivas = 0;
  while (leiturasConsecutivas < quantidade) {
    float leitura = leituraSensor(argumento); // Faz a leitura com argumento
    if (leitura < comparador) {
      leiturasConsecutivas++;
    } else {
      return false; // Leitura inválida
    }
  }
  return true; // Todas as leituras foram válidas
}
