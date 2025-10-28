void setupPid(); // Inicia os pinos de controle para o multiplexador e o botão de calibração
void pidCalib(); // Calibração dos sensores de linha, sem EEPROM. Segure o botão para calibrar
void selectChannel(int channel); // Seleciona o canal do multiplexador
void defineLuz(); // Define a luminosidade dos infras
void calculoPesos(); // Calcula os pesos para cada sensor com base em uma função exponencial. Isso dá mais importância aos sensores mais distantes do centro, gerando uma resposta mais agressiva

// Funções para salvar e carregar os valores de calibração na EEPROM
void gravarEeprom(); 
void lerEeprom(); 

void fator_I(); // O fator I do PID

void fazerLeitura(); // Lê os valores brutos dos sensores e os converte em valores digitais (preto ou branco) e analógicos (escala de 0 a 1)
void calcularLeitura(); // Processa os dados lidos para calcular o erro atual do robô em relação à linha
void PID(); // Calcula os pesos e a movimentação

// Armazenam os valores de branco e preto lidos para cada um dos 16 sensores durante a calibração
int valorMaximo [16];
int valorMinimo [16];

// Guardam as leituras atuais dos sensores em formato analógico (0 a 1) e digital (0 ou 1)
float valoresAnalogicos [16];
int valoresDigitais [16];

// Vetor com os pesos calculados para cada sensor, usados para determinar a posição do robô em relação à linha
int pesos [16];

// Representam a posição atual do robô
float media_analogico = 0;
int media_digital = 0;

// A diferença entre a posição atual do robô e o centro da linha
float erro = 0;

// Componentes do PID
float proporcional = 0.0;
float integral = 0.0;
float derivativo = 0;

// Valores de correção de velocidade para os motores esquerdo e direito
float diferencial = 0;
float diferencial_2 = 0;

// Variáveis booleanas usadas para gerenciar o estado do robô, como a perda da linha e a direção da última perda
bool validador1 = true;
bool validador2 = true;
bool validador3 = false;

// Flags para indicar se o robô está em uma borda da linha ou em uma situação de limite de tempo para correções
bool noLimite = false;
bool comLimite = false;
bool tempLimite = false;
unsigned long tempoNoLimite = 0;
unsigned long tempoComLimite = 0;
unsigned long tempoLadoEsq = 0;
unsigned long tempoLadoDir = 0;

int lado = 0; // Armazena a direção da última vez que o robô perdeu a linha
int dif_maximo = 0; // Usado para controlar a amplitude da correção do PID

// Usadas para controlar o tempo de pulso dos motores, criando um movimento intermitente (com paradas)
long tempo_pid = 0;
float diferencial_temp = 0;
float diferencial_2_temp = 0;

const int plex_calib = 40; // Valor do preto, varia de 0 a 100 [+ sensibilidade para o 100]

// Ganhos do controle PID para a parte Proporcional e Derivativa
const float KP = 1.0;
//const float KI = 0;
const float KD = 0.0;

// Constantes que definem a velocidade máxima dos motores para a frente e para trás
int vel_maxima = 80; // Vel máxima frente
int vel_maxima_2 = 200; // Vel máxima frente primeiro setor
int vel_maxima_3 = 255; // Vel máxima frente segundo setor
int vel_limite = 255; // Vel máxima trás

int zona_morta = 60; // A velocidade mínima para o robô se mover: 60 para ir para frente e 100 para girar

// Proporções da velocidade
long tempo1 = 1;
long tempo2 = 30; //32 //45

const double expoente = 0.01; // O expoente usado na função que calcula os "pesos" dos sensores. Um valor menor resulta em uma curva de resposta mais agressiva (0.3)
float setpoint = 0; // O valor ideal (ponto central) da linha, que o robô tenta manter

const int nivel = 10; // Regula a luminosidade dos infras (0 - 31)
const int nivel_maximo = 31;

union{ int inteiro; byte bite[2];} conversor; // Permite que um valor inteiro de 16 bits seja tratado como dois bytes separados, o que é útil para salvar valores na memória EEPROM, que é endereçada em bytes

extern bool pidRevertido;
extern bool rampa_ativa;

// Variáveis para um modo de controle alternativo, possivelmente para curvas especiais ou desvios
long PID_alt = 0;
long PID_dois_verdes = 0;
bool PID_alt_temp = false;
bool PID_esq = false;
bool PID_dir = false;

long erro_anterior = 0;
long tempo_erro = 0;
const long erro_maximo = 6;
const int tempo_ativacao = 2000; // Limite de tempo para ativar o I //2000
bool comI = false;
int tempo2_temp = 0;

void resetPid(); // Declaração cruzada com MUV_Lado


void setupPid(){
  pinMode(plex0, OUTPUT);
  pinMode(plex1, OUTPUT);
  pinMode(plex2, OUTPUT);
  pinMode(plex3, OUTPUT);
  pinMode(botao_calib, INPUT_PULLUP); // Ativa o resistor interno do arduino para o pino [Positivo enquanto não pressionado]
  pinMode(plex_even, OUTPUT);
  pinMode(plex_odd, OUTPUT);
  digitalWrite(plex_even, HIGH);
  digitalWrite(plex_odd, HIGH);
}



void selectChannel(int channel){
  digitalWrite(plex0, bitRead(channel, 0));
  digitalWrite(plex1, bitRead(channel, 1));
  digitalWrite(plex2, bitRead(channel, 2));
  digitalWrite(plex3, bitRead(channel, 3));
}



void defineLuz(){ // Ajusta o brilho dos sensores
  // Reset dos dois sensores
  digitalWrite(plex_even, LOW);
  digitalWrite(plex_odd, LOW);
  delay(5);
  digitalWrite(plex_even, HIGH);
  digitalWrite(plex_odd, HIGH);
  delayMicroseconds(50);
  // Calcula quantos pulsos
  int pulsos = nivel_maximo - nivel;
  for (int i = 0; i < pulsos; i++) {
    digitalWrite(plex_even, HIGH);
    digitalWrite(plex_odd, HIGH);
    delayMicroseconds(10);
    digitalWrite(plex_even, LOW);
    digitalWrite(plex_odd, LOW);
    delayMicroseconds(10);
  }
  // Latch final
  digitalWrite(plex_even, HIGH);
  digitalWrite(plex_odd, HIGH);
  delayMicroseconds(2000);
}



void pidCalib(){
  defineLuz();
  if(digitalRead(botao_calib) == LOW){ // Botão pressionado
    digitalWrite(led_esq_R, HIGH);
    digitalWrite(led_dir_R, HIGH);
    while(digitalRead(botao_calib) == LOW)
    delay(1000);
    digitalWrite(led_esq_G, HIGH);
    digitalWrite(led_dir_G, HIGH);
    Serial.println("Inicio da calibração");
    for(int i = 0; i < 16; i++){
      valorMaximo [i] = 0;
      valorMinimo [i] = 1023;
    }
    while(digitalRead(botao_calib) == HIGH){ // Botão da calibração
      for(int i = 0; i < 16; i++){
        selectChannel(i);
        int temp = analogRead(plex_leitura); // Lê os dados do sensor
        if(temp > valorMaximo[i]){
          valorMaximo[i] = temp;
        }
        if(temp < valorMinimo[i]){
          valorMinimo[i] = temp;
        }
      }
    }
    gravarEeprom();
    delay(1000);
    digitalWrite(led_esq_R, LOW);
    digitalWrite(led_dir_R, LOW);
    Serial.println("Fim da calibração");
    while(true);
  }
  else{ lerEeprom();}
  calculoPesos(); 
}



void calculoPesos(){ // Calcular os pesos dos sensores
  for (int i = 7; i >= 0; i--) { // Calcula os pesos do lado esquerdo [negativos]
    pesos[-i + 7] = (pow(expoente, i) - 1) * 1000; // Exponenciação gerada pelo primeiro fator do "pow", modifique para alterar a curva, com cuidado
    Serial.println(pesos[-i + 7]);
  }
  for (int i = 0; i <= 7; i++) { // Calcula os pesos do lado direito[positivos]
    pesos[i + 8] = -(pow(expoente, i) - 1) * 1000; // Utilize o geogebra caso queira ver o gráfico da função
    Serial.println(pesos[i + 8]);
  }
  dif_maximo = pesos[14];
}



void gravarEeprom(){
  if(!digitalRead(toque_esq) || !digitalRead(toque_dir)){
    EEPROM.write(64, 0);
    EEPROM.write(65, 0);
    digitalWrite(led_esq_B, HIGH);
    digitalWrite(led_dir_B, HIGH);
    while(true);
  }
  for(int i = 0; i < 32; i+=2){
    conversor.inteiro = valorMaximo[i / 2];
    EEPROM.write(i, conversor.bite[0]); 
    EEPROM.write(i + 1, conversor.bite[1]);
  }
  for(int i = 0; i < 32; i+=2){
    conversor.inteiro = valorMinimo[i / 2];
    EEPROM.write(i + 32, conversor.bite[0]); 
    EEPROM.write(i + 33, conversor.bite[1]);
  }
  EEPROM.write(64, 0);
  EEPROM.write(65, 0);
}



void lerEeprom(){
  for(int i = 0; i < 32; i+=2){
    conversor.bite[0] = EEPROM.read(i);
    conversor.bite[1] = EEPROM.read(i + 1);
    valorMaximo[i / 2] = conversor.inteiro;
  }
  for(int i = 0; i < 32; i+=2){
    conversor.bite[0] = EEPROM.read(i + 32);
    conversor.bite[1] = EEPROM.read(i + 33);
    valorMinimo[i / 2] = conversor.inteiro;
  }
}



void fator_I(){
  if(abs(round(erro) - erro_anterior) < erro_maximo){
    if(tempo_erro == 0){
      tempo_erro = millis();
      tempo2_temp = tempo2;
      resetPid();
    }
    if(millis() - tempo_erro >= tempo_ativacao){
      tempo2 = tempo2_temp - round((millis() - (tempo_erro + tempo_ativacao)) / 75);
      //vel_maxima = vel_maxima + round((millis() - (tempo_erro + tempo_ativacao)) / 50);
      if (tempo2 < 0){ tempo2 = 0;}
      //if (vel_maxima > 255){ vel_maxima = 255;}
    }
  } 
  else{
    if(tempo_erro != 0){
      tempo_erro = 0;
      resetPid();
    }
  }
  erro_anterior = round(erro);
}



void fazerLeitura(){ // Converte os valoes para 0 (branco) ou 1 (preto)
  for(int i = 0; i < 16; i++){
    selectChannel(i);
    int leitura = analogRead(plex_leitura);
    valoresAnalogicos[i] = constrain((constrain(map(leitura, valorMinimo[i], valorMaximo[i], 0, 100), 0, 100) / 100.0), 0.0, 1.0); // Constrain extra por segurança (irrelevante)
    temp = constrain(map(leitura, valorMinimo[i], valorMaximo[i], 0, 100), 0, 100);
    if(temp > plex_calib){
      valoresDigitais[i] = 0;
    }else{
      valoresDigitais[i] = 1;
    }
    /*
    Serial.print(valoresDigitais[i]);
    Serial.print(" ");
    */
  }
}



// Media digital < 0 -> linha à esquerda, =~ 0 -> linha centralizada, > 0 -> linha à direita
void calcularLeitura(){
  float somaPesoAnalogico = 0;
  float somaCruAnalogico = 0;
  int somaPesoDigital = 0;
  int somaCruDigital = 0;

  for(int i = 1; i < 15; i++){ // Calcula a soma total das leituras
    somaPesoAnalogico += valoresAnalogicos[i] * pesos[i];
    somaCruAnalogico += valoresAnalogicos[i];
    somaPesoDigital += valoresDigitais[i] * pesos[i];
    somaCruDigital += valoresDigitais[i];
  }
  
  media_analogico = somaPesoAnalogico/(somaCruAnalogico + 1); // Calcula as médias
  if(somaCruDigital == 0){ media_digital = -1;} // Dá o sinal de que a linha foi perdida
  else{ media_digital = somaPesoDigital/somaCruDigital;}

  if(valoresDigitais[1] == 1){tempoLadoEsq = millis();}
  if(valoresDigitais[14] == 1){tempoLadoDir = millis();}
  if(valoresDigitais[1] == 1 || valoresDigitais[14] == 1){ // Ativa "noLimite" quando a linha estiver parcial ou totalmente em um dos extremos
    noLimite = true;
    tempoNoLimite = millis() + 250; //150
    if(valoresDigitais[1] == 1){
      validador3 = true;
    }
    else{
      validador3 = false;
    }
  }
  if(tempoNoLimite < millis()){ // Zera "noLimite" quando acabar o tempo
    noLimite = false;
  }

  if(tempoComLimite >= millis()){
    comLimite = true;
    tempLimite = true;
  }
  else if(tempLimite){
    comLimite = false;
    tempLimite = false;
  }
  
  if(media_digital > -1 && validador1){ // Atua na troca do lado ativo[o lado onde a linha está ou estava], evita reativações constantes[meio irrelevante]
    lado = 0;
    validador1 = false;
    validador2 = true;
  }
  if(media_digital < -1 && validador2){ 
    lado = 1;
    validador1 = true;
    validador2 = false;
  }
  // /*
  Serial.print("lado/");
  Serial.print(lado);
  Serial.print(" ");
  // */
  if(media_digital == -1 && noLimite && !comLimite){ // Ativa o modo turbo se a linha for perdida ate à achar [Se perdeu a linha e se recentemente viu a linha com os sensores da ponta]
    if(tempoLadoEsq < tempoLadoDir){
      media_analogico = dif_maximo;
      media_digital = dif_maximo;
    }
    else{
      media_analogico = -dif_maximo;
      media_digital = -dif_maximo;
    }
    tempoNoLimite = millis() + 500;
  }
  /*
  if(media_digital == -1 && noLimite && !comLimite){ // Ativa o modo turbo se a linha for perdida ate à achar [Se perdeu a linha e se recentemente viu a linha com os sensores da ponta]
    //if(!validador3){
    if(erro > -1 && lado == 0){
      media_analogico = dif_maximo;
      media_digital = dif_maximo;
      erro = 2;
    }else if(erro < -1 && lado == 1){
      media_analogico = -dif_maximo;
      media_digital = -dif_maximo;
      erro = -3;
    }else{
      /*
      digitalWrite(led_esq_R, HIGH);
      //digitalWrite(led_esq_G, HIGH);
      digitalWrite(led_dir_R, HIGH);
      //digitalWrite(led_dir_G, HIGH);
      moveMotores(-200, -200);
      delay(150); //150
      if(!(erro > -1)){
        moveMotores(150, -150);
        delay(75); //100
      }else{
        moveMotores(-150, 150);
        delay(75); //100
      }
      erro = -1;
      limpaLeds();
    }  
    tempoNoLimite = millis() + 500;
  }
  */
  
  if(media_digital == -1 && !noLimite){ // Failsafe caso a linha se perca sem passar pelas laterais [gap?]
    media_analogico = 0;
    media_digital = 0;
    nLinha = true;
  }
}



void PID(){
  // PIDs alternativos
  if(millis() < PID_alt){
    if(PID_esq){
      setpoint = round(pesos[14] / 2 * -1);
      PID_alt_temp = true;
    }
    else{ //dir
      setpoint = round(pesos[14] / 2); // 2
      PID_alt_temp = true;
    }
  }
  else{
    setpoint = 0;
    if(PID_alt_temp){
      resetPid();
      PID_alt_temp = false;
    }
  }
  
  fazerLeitura();
  calcularLeitura();
  if(comI){
    fator_I();
  }
  
  //  Serial.print("media_digital/");
  //  Serial.print(media_digital);
  //  Serial.print(" ");
  
  proporcional = media_digital - setpoint; // Calcula o quão longe o robô está do ponto de referência. Quanto maior a distância, maior a correção
  derivativo = erro - proporcional; // Calcula a velocidade com que o erro está mudando. Isso ajuda a prever e reagir a mudanças bruscas, como curvas fechadas
  erro = erro * 0.99 + proporcional * 0.01; // maneira de suavizar o erro, dando mais peso ao erro anterior (95%) e um pouco do erro atual (5%), tornando a resposta mais estável
  diferencial = (proporcional*KP) + (derivativo*KD); 

  //  Serial.print("dif_old/");
  //  Serial.print(diferencial);
  //  Serial.print(" ");
  
  //  if(rampa_ativa && noLimite && false){
  //    if(valoresDigitais[1] == 1){
  //      diferencial += -dif_maximo / 2;
  //    }
  //    else{
  //      diferencial += dif_maximo / 2;
  //    }
  //  }

  if(comLimite && !tempLimite){
    diferencial = diferencial / 2;
    diferencial_2 = diferencial_2 / 2;
  }

  // Toda vez que o diferencial passar a velocidade máxima, este será definido como a nova velocidade maxima [PID Adaptativo Dinâmico]
  int temp = diferencial; // Salva o valor do diferencial em uma variável temporária.
  if(abs(diferencial) <= dif_maximo / 2){ // Erro pequeno = alta velocidade
    diferencial = map(abs(diferencial), 0, dif_maximo / 2, vel_maxima, zona_morta);
    diferencial_2 = map(abs(diferencial), 0, dif_maximo / 2, vel_maxima, vel_maxima_2);
  } else{ // Erro grande
    if(pidRevertido){
      //diferencial = map(abs(diferencial), dif_maximo / 2, dif_maximo, zona_morta, -vel_limite);
      diferencial = map(abs(diferencial), dif_maximo / 2, dif_maximo, zona_morta, zona_morta);
    }
    else{
      diferencial = map(abs(diferencial), dif_maximo / 2, dif_maximo, -zona_morta, -vel_limite);
    }
    diferencial_2 = map(abs(diferencial), dif_maximo / 2, dif_maximo, vel_maxima_2, vel_maxima_3);
  }
  
  if(diferencial > vel_maxima){
    //dif_maximo = abs(temp);
    diferencial = vel_maxima;
  }
  if(diferencial < -vel_limite){
    //dif_maximo = abs(temp);
    diferencial = -vel_limite;
  }
  
  else if(tempo_pid + tempo2 > millis()){
    diferencial = 0;
    diferencial_2 = 0;
  }
  else{
    tempo_pid = millis();
    diferencial_temp = diferencial;
    diferencial_2_temp = diferencial_temp;
  }
   
  if(temp < 0){
    moveMotores(diferencial, diferencial_2);
  }else{
    moveMotores(diferencial_2, diferencial);
  }
  Serial.println();
}
