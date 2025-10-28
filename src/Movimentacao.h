enum tipo_angulo {PITCH, YAW, ROLL}; // Define um agrupamento tipo enum (Melhor que string)
enum ModoDiferenca { SEM_MODULO, MODULO };

void moveMotores(int speed_esq, int speed_dir, int multi = 0); // Movimenta o robô (velocidade esquerda, velocidade direita, Usar ou não o multiplicador)
void moveMotoresEncoder(int speed_esq, int speed_dir); // Movimenta o robô com o encoder
// Interrupção do encoder
void encoderISREsq(); 
void encoderISRDir(); 
void Stop(); // Para o robô
void MRUAngular(int angulo, int vel); // Movimentação por ângulo (ângulo, velocidade) [MRU - Movimento Retilíneo Uniforme]
int angular(tipo_angulo angulo); // Leitura do ângulo atual (Tipo do Ângulo) {PITCH, YAW, ROLL}
int difAngular(int, int, ModoDiferenca modo); // Menor diferença entre dois ângulos (Angulo inicial, Angulo atual, Módulo[opcional]) {MODULO}

volatile long encoderEsq = 0; // Encoder
volatile long encoderDir = 0;
long imuEncoder = 0;
int erroEncoder = 0;
long coder_esq_old = 0;
long coder_dir_old = 0;
float fator_de_acrescimo = 1.0;
bool positivo = true;

extern long imuVerde;
extern long imu90;
/* 
 * Velocidade máxima - 
 * Velocidade mínima -
 * 
 * PITCH - Rotação vertical
 * YAW - Rotação horizontal - Movimentação baseada no modelo horário: Esq=+ Dir=- (circulo trigonométrico distoa de todo o resto do código)
 * ROLL - Rotação lateral - Esq negativo e Dir positivo
 */

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // Inicializa o sensor de ângulo. (Numero arbitrário, Endereço I2C(GND - 0x28/VCC - 0x29))
sensors_event_t event; // Algo da biblioteca



void moveMotores(int speed_esq, int speed_dir, int multi){
  // /*
  if(speed_dir >= 0){
    digitalWrite(MdirA, LOW);
    analogWrite(MdirB, speed_dir);
  }else{
    analogWrite(MdirA, abs(speed_dir));
    digitalWrite(MdirB, LOW);
  }
  if(speed_esq >= 0){
    digitalWrite(MesqA, LOW);
    analogWrite(MesqB, speed_esq * pow(1.17, multi)); // Corrigir a diferença de velocidade dos motores
  }else{
    analogWrite(MesqA, abs(speed_esq * pow(1.17, multi)));
    digitalWrite(MesqB, LOW);
  }
  // */
}

void moveMotoresEncoder(int speed_esq, int speed_dir){
  if(imuEncoder <= millis()){
    erroEncoder = 0;
    coder_esq_old = encoderEsq;
    coder_dir_old = encoderDir;
  }
  noInterrupts();
  long tempEsq = encoderEsq;
  long tempDir = encoderDir;
  interrupts();
  long dif_esq = tempEsq - coder_esq_old;
  long dif_dir = tempDir - coder_dir_old;
  int dif = dif_esq - dif_dir;
  erroEncoder += dif;
  if(erroEncoder >= 0){ positivo = true;} else{ positivo = false;}
  coder_esq_old = tempEsq;
  coder_dir_old = tempDir;
  // /*
  if(speed_dir >= 0){
    digitalWrite(MdirA, LOW);
    if(positivo){ analogWrite(MdirB, constrain(speed_dir + (erroEncoder * fator_de_acrescimo), 0, 255));} else{ analogWrite(MdirB, speed_dir);}
  }else{
    if(positivo){ analogWrite(MdirA, abs(speed_dir));} else{ analogWrite(MdirA, constrain(abs(speed_dir) - (erroEncoder * fator_de_acrescimo), 0, 255));}
    digitalWrite(MdirB, LOW);
  }
  if(speed_esq >= 0){
    digitalWrite(MesqA, LOW);
    if(positivo){ analogWrite(MesqB, speed_esq);} else{ analogWrite(MesqB, constrain(speed_esq - (erroEncoder * fator_de_acrescimo), 0, 255) * 1.2);}
  }else{
    if(positivo){ analogWrite(MesqA, constrain(abs(speed_esq) + (erroEncoder * fator_de_acrescimo), 0, 255)  * 1.2 );} else{ analogWrite(MesqA, abs(speed_esq));}
    digitalWrite(MesqB, LOW);
  }
  // */
  Serial.print((speed_esq) + (erroEncoder * fator_de_acrescimo));
  Serial.print(" ");
  Serial.print(speed_dir + (erroEncoder * fator_de_acrescimo));
  imuEncoder = millis() + 100;
}

void encoderISREsq() {
  if (digitalRead(coder_esq_A) != HIGH) {
    encoderEsq++;
  } 
  else {
    encoderEsq--;
  }
}

void encoderISRDir() {
  if (digitalRead(coder_dir_A) != HIGH) {
    encoderDir--;
  } 
  else {
    encoderDir++;
  }
}

void Stop(){
  Stop();
}

void MRUAngular(int angulo, int vel){
  bno.getEvent(&event);
  int angulo_atual = event.orientation.x;
  int angulo_desejado = (angulo_atual + angulo) % 360; // Corrige o loop do 360/0
  if (angulo_desejado < 0) angulo_desejado += 360; // Ajusta ângulos negativos

  if (angulo > 0) {
    moveMotores(vel, -vel); // Positivo → sentido horário
  } else {
    moveMotores(-vel, vel); // Negativo → sentido antihorário
  }

  int erro = 5;
  do { // Aguarda atingir o ângulo desejado, considerando margem de erro
    bno.getEvent(&event);
    angulo_atual = event.orientation.x;
  } while (!(angulo_atual >= angulo_desejado - erro && angulo_atual <= angulo_desejado + erro));

  Stop();
}

int temp = 0;
int angular(tipo_angulo angulo) // int angular(Possibilidades de resposta, a resposta)
{
  sensors_event_t event;
  bno.getEvent(&event);
  
  switch (angulo) {
    case PITCH:
      return event.orientation.y;
    case YAW:
      if(event.orientation.x > 0){
        temp = event.orientation.x;
        return event.orientation.x;
      }
      else{ return temp;}
      //return event.orientation.x;
    case ROLL:
      temp = event.orientation.z;
      if (temp >= 0)
        return temp - 180;
      else if (temp < 0)
        return temp + 180;
    default:
      return 0; // o Erro
  }
}

int difAngular(int angulo_inicial, int angulo_final, ModoDiferenca modo = SEM_MODULO) {
  int diferenca_horario = (angulo_inicial - angulo_final + 360) % 360; // Sentido horário
  int diferenca_anti_horario = (angulo_final - angulo_inicial + 360) % 360; // Sentido antihorário

  if (modo == MODULO) {
    return min(diferenca_horario, diferenca_anti_horario); // Retorna o menor valor entre os dois
  } else {
    if (diferenca_horario <= diferenca_anti_horario) {
      return diferenca_horario;  // Sentido horário → positivo
    } else {
      return -diferenca_anti_horario;  // Sentido antihorário → negativo
    }
  }
}
