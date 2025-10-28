#define t_servo 500 // Tempo padrão de movimento dos servos

// Funções de sensores
int infra(int canal); // Retorna o valor de um dos infras (n° do sensor) [Requer PID calib] {0-Esq / (-1)-Meio / 15-Dir}
int ultra(int lado); // Retorna a leitura do ultrassônico (lado do sensor) {ESQu, DIRu, FRENTEu}
void tofPlex(int lado); // Plexamento do Tof
int tof(int lado); // Retorna a leitura do Tof (lado do sensor) {ESQt, DIRt, FRENTEt}
int tcsCor(); // Retorna a cor vista {LUZ = 0, RED = 1, GREEN = 2, BLUE = 3}

// Funções de atuadores
void sobeGarra(int tempo = t_servo); // [Velocidade como parâmetro opcional, -1 não desatarraxa os servos e ignora o tempo]
void desceGarra(int tempo = t_servo); // [Velocidade como parâmetro opcional, -1 não desatarraxa os servos e ignora o tempo]
void abreGarra(int tempo = t_servo);
void fechaGarra(int tempo = t_servo);
void abreGarraEsq(int tempo = t_servo);
void abreGarraDir(int tempo = t_servo);
void vitimaNeutro(int tempo = t_servo);
void vitimaViva(int tempo = t_servo);
void vitimaMorta(int tempo = t_servo);

// Funções gerais
void recLinha(); // Recupera linha
void verde90(); // Interliga verde, 90, e imunidade ao verde. Usar como função principal
void fim(); // Chegou no vermelho

// Funções genéricas


void verde(); // Definição em Verde.h
void curva90(); // Definição em Curva90.h
void curva90Rec();
void selectChannel(int channel); // Definição em PID.h
void PID();
void encoder90();

extern int valorMaximo [16]; // Valores máximos calibrados dos sensores IR
extern int valorMinimo [16]; // Valores mínimos calibrados dos sensores IR
extern unsigned long tempoComLimite; // Controle de tempo com limite

VL53L0X lox; // Define o Tof
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // Define o Tcs

long imuVerde = 0;
long imu90 = 0;
bool imuEncoder90 = false;
long imuEncoder90_2 = 0;

int infra(int canal){
  if(canal == -1){
    int temp1 = infra(7);
    int temp2 = infra(8);
    return((temp1 + temp2) / 2);
  }
  if(canal == -2){
    int temp = 0;
    for(int i = 2; i < 14; i++){
      if(reLeitura(infra, i, in_preto, 20)){
        return(true);
      }
    }
    return(false);
  }
  selectChannel(canal);
  int leitura = analogRead(plex_leitura);
  leitura = map(leitura, valorMinimo[canal], valorMaximo[canal], 0, 100); // Constrain extra por segurança (irrelevante)
  return(leitura);
}

int ultra(int lado) { // ENUM LadoSensorUltra
  int trigPino, echoPino;
  switch (lado) {
    case ESQu:
      trigPino = ultra_trig_esq;
      echoPino = ultra_echo_esq;
      break;
    case DIRu:
      trigPino = ultra_trig_dir;
      echoPino = ultra_echo_dir;
      break;
    case FRENTEu:
      trigPino = ultra_trig_frente;
      echoPino = ultra_echo_frente;
      break;
  }
  digitalWrite(trigPino, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPino, LOW);
  float duracao_us = pulseIn(echoPino, HIGH, 4412);
  if(duracao_us == 0) {duracao_us = 4412;}
  float distancia_cm = 0.017 * duracao_us;
  delay(10);
  //Serial.println(distancia_cm);
  return (distancia_cm);
}

void tofPlex(int lado){
  Wire.beginTransmission(0x77);
  Wire.write(1 << lado);
  Wire.endTransmission();
  delay(10);
}

int tof(int lado){
  tofPlex(lado); // lado será 3 (ESQ), 2 (DIR) ou 1 (FRENTE)
  int leitura = lox.readRangeSingleMillimeters();
  return (leitura);
}

int tcsCor(){
  digitalWrite(led_tcs, HIGH);
  delay(60);
  tofPlex(0);
  float red, green, blue;
  tcs.getRGB(&red, &green, &blue);
  digitalWrite(led_tcs, LOW);
  if(green/red >= 2){
    return(2);
  } else if(green/red < 1){
    return(1);
  } else{
    return(0);
  }
  return(green/red);
}

void sobeGarra(int tempo){
  braco.attach(braco_pin);
  braco.write(158);
  if(tempo != -1){
    delay(tempo * 2);
    braco.detach();
  }
}

void desceGarra(int tempo){
  braco.attach(braco_pin);
  braco.write(15); // 10
  if(tempo != -1){
    delay(tempo * 2);
    braco.detach();
  }
}

void abreGarra(int tempo){
  garra_esq.attach(garra_esq_pin);
  garra_dir.attach(garra_dir_pin);
  garra_esq.write(115);
  garra_dir.write(42);
  delay(tempo);
  garra_esq.detach();
  garra_dir.detach();
}

void fechaGarra(int tempo){
  garra_esq.attach(garra_esq_pin);
  garra_dir.attach(garra_dir_pin);
  garra_esq.write(54); // 47
  garra_dir.write(106); // 112
  delay(tempo);
}

void abreGarraEsq(int tempo){
  garra_esq.attach(garra_esq_pin);
  garra_dir.attach(garra_dir_pin);
  garra_esq.write(112);
  garra_dir.write(80);
  delay(tempo);
  for (int i = 0; i < 3; i++) {
    garra_dir.write(127); //121
    delay(tempo / 5);
    garra_dir.write(96); //90
    delay(tempo / 5);
  }
  garra_esq.detach();
  garra_dir.detach();
}

void abreGarraDir(int tempo){
  garra_esq.attach(garra_esq_pin);
  garra_dir.attach(garra_dir_pin);
  garra_dir.write(42);
  delay(tempo);
  for (int i = 0; i < 3; i++) {
    garra_esq.write(20); //32
    delay(tempo / 5);
    garra_esq.write(51); //63
    delay(tempo / 5);
  }
  garra_esq.detach();
  garra_dir.detach();
}

void vitimaNeutro(int tempo){
  vitima.attach(vitima_pin);
  vitima.write(81);
  delay(tempo);
  delay(500);
  vitima.detach();
}

void vitimaMorta(int tempo){
  vitima.attach(vitima_pin);
  vitima.write(165);
  delay(tempo);
  delay(500);
  vitima.detach();
}

void vitimaViva(int tempo){
  vitima.attach(vitima_pin);
  vitima.write(0);
  delay(tempo);
  delay(500);
  vitima.detach();
}

long imuRec = 0; // Controlar a imunidade da recuperação de linha
extern float erro; // Erro atual do PID
extern int dif_maximo; // Diferença máxima de erro
 
void recLinha(){
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_dir_G, HIGH);
  moveMotores(-20, -20);
  delay(250);
  bool linha = false; // Indica se a linha foi encontrada
  while(true){
    moveMotores(100, 100);
    repitaPorTempo(250){ if(infra(-2)){ linha = true; break;}} // Andar pra frente por 250ms procurando linha. Caso ache-a, quebre o loop
    if(linha){ break;}
    Stop();
    delay(250);
    /*
    moveMotores(-100, -100);
    repitaPorTempo(250){ if(infra(-2)){ linha = true; break;}} // Andar pra trás por 250ms procurando linha. Caso ache-a, quebre o loop
    if(linha){ break;}
    */
    
    int angulo = angular(YAW);

   moveMotores(150, -150);
    while(difAngular(angulo, angular(YAW)) > -95){if(infra(-2)){linha = true; break;}} // // Girar pra direita por 75º procurando linha. Caso ache-a, quebre o loop
    if(linha){ break;}
    Stop();
    delay(250);
    
    moveMotores(-150, 150);
    while(difAngular(angulo, angular(YAW)) < 75){if(infra(-2)){ linha = true; break;}} // // Girar pra esquerda por 75º procurando linha. Caso ache-a, quebre o loop
    if(linha){ break;}
    Stop();
    delay(250);
    
    MRUAngular(70, 150); // Retornar a posição inicial
    delay(250);
    Stop();
    delay(250);
  }
  // Após encontrar a linha, recua um pouco para se centralizar
  moveMotores(-100, -100);
  delay(150);
  digitalWrite(led_esq_R, LOW);
  digitalWrite(led_esq_G, LOW);
  digitalWrite(led_dir_R, LOW);
  digitalWrite(led_dir_G, LOW);
  imuRec = millis() + 1000; // Define um período de imunidade para evitar outra recuperação imediata
  tempoComLimite = millis() + 500; // Reseta o tempo limite para a próxima PID
}

void verde90(){
  curva90(); // Verifica se estou em intersecção e devo ignorar verdes
  if(millis() < imuVerde){
    PID();
  }
  else{
    verde();
  }
}

void fim(){
  Stop();
  for (int i = 0; i < 16; i++) {
    digitalWrite(led_esq_R, HIGH);
    digitalWrite(led_dir_R, HIGH);
    delay(250);
    digitalWrite(led_esq_R, LOW);
    digitalWrite(led_dir_R, LOW);
    delay(250);
  }
  moveMotores(-255, -255);
  delay(100);
  Stop();
}
