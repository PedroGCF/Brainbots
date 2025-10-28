void subSetup(); // Função principal de configuração do robô
void setupTPB(); // Configura os sensores de toque, sensor de prata e buzzer
void setupSerialI2C(); // Inicializa comunicações Serial e I2C
void setupMotor(); // Configura os pinos dos motores
void setupEncoder(); // Configura os encoders e suas interrupções
void setupBNO(); // Inicia o sensor de ângulo pró
void setupInfra(); // Configura os sensores infravermelhos
void setupLeds(); // Inicializa todos os LEDs do robô
void setupGarra(); // Posiciona a garra e o servo da vítima em estado inicial seguro
void setupUltra(); // Configura os sensores ultrassônicos
void setupToF(); // Inicializa os sensores ToF
void setupTcs(); // Inicializa o sensor de cor TCS34725
void setupServo(); // Associa os objetos Servo aos pinos físicos

void subSetup(){
  setupTPB();
  setupSerialI2C();
  setupMotor();
  setupEncoder();
  setupLeds();
  setupUltra();
  apdsSetup();
  setupBNO();
  setupInfra();
  setupToF();
  setupPid();
  Serial3.begin(115200);
  setupServo();
  setupGarra();
  
  pinMode(A13, INPUT);
  pidCalib();
  
  Serial.println("Arduino Iniciado");
  delay(1000);
}

void setupInfra(){
  pinMode(in_esq, INPUT);
  pinMode(in_meio, INPUT);
  pinMode(in_dir, INPUT);
}

void setupEncoder(){
  pinMode(18, INPUT);
  pinMode(coder_esq_A, INPUT);
  attachInterrupt(digitalPinToInterrupt(coder_esq_B), encoderISREsq, RISING);
  pinMode(19, INPUT);
  pinMode(coder_dir_A, INPUT);
  attachInterrupt(digitalPinToInterrupt(coder_dir_B), encoderISRDir, RISING);
}

void setupBNO(){
  bno.begin(); // Inicia o Sensor
  delay(200); 
  bno.setExtCrystalUse(true); // Usa o cristal externo do BNO055 para maior precisão
}

void setupLeds(){
  pinMode(led_tcs, OUTPUT);
  pinMode(led_esq_R, OUTPUT);
  pinMode(led_esq_G, OUTPUT);
  pinMode(led_esq_B, OUTPUT);
  pinMode(led_dir_R, OUTPUT);
  pinMode(led_dir_G, OUTPUT);
  pinMode(led_dir_B, OUTPUT);
}

void setupMotor() {
  pinMode(MesqB, OUTPUT);
  pinMode(MesqA, OUTPUT);
  pinMode(MdirB, OUTPUT);
  pinMode(MdirA, OUTPUT);
}

void setupTPB(){
  pinMode(toque_esq, INPUT_PULLUP);
  pinMode(toque_dir, INPUT_PULLUP);
  pinMode(prata_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
}

void setupGarra(){
  sobeGarra(-1);
  abreGarra();
  vitimaNeutro(0);
  delay(500);
  sobeGarra(0);
}

void setupSerialI2C(){
  Serial.begin(115200); // Inicia a comunicação serial 
  Wire.begin(); // Inicia o protocolo I2C (BNO e APDS)
  Wire.setClock(400000); // Define a velocidade de comunicação do I2C
}

void setupUltra() {
  pinMode(ultra_trig_esq, OUTPUT);
  pinMode(ultra_echo_esq, INPUT);
  pinMode(ultra_trig_dir, OUTPUT);
  pinMode(ultra_echo_dir, INPUT);
  pinMode(ultra_trig_frente, OUTPUT);
  pinMode(ultra_echo_frente, INPUT);
}

void setupToF(){
  for(int i = 1; i <= 3; i++){
    tofPlex(i);
    lox.setTimeout(100);
    lox.init();
  }
}

void setupTcs(){
  tofPlex(0);
  tcs.begin();
}

void setupServo(){
  garra_esq.attach(garra_esq_pin);
  garra_dir.attach(garra_dir_pin);
  vitima.attach(vitima_pin);
}
