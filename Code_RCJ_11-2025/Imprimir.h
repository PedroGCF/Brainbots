//A ordem é respectiva à sequência
void inInfra(); // Esquerda, meio e direita
void inCor(); //  Esquerda e direita
void inVerde(); // Esquerda e direita
void inVermelho(); // Esquerda e direita
void inAngulo(); // Pitch, Yaw e Roll
void inUltra(); // Esquerda, meio e direita
void inPid(); // Esquerda para direita
void inPidAnalog(); // Esquerda para direita
void inPidDigital(); // Esquerda para direita
void inPidPesos(); // Esquerda para direita
void inTof(); // Esquerda, meio e direita
void inCom(); // Ordem dos seriais
void scanI2c(); // Dá ping nos endereços i2c do arduino

void inInfra(){
  Serial.print(infra(0));
  Serial.print(" / ");
  Serial.print(infra(-1));
  Serial.print(" / ");
  Serial.println(infra(15));
}

void inCor(){
  Serial.print(sCor(ESQ, LUZ));
  Serial.print(" / ");
  Serial.println(sCor(DIR, LUZ));
}

void inVerde(){
  apdsPlex(ESQ);
  apds.isAlsAvailable();
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  float proporcao_esq = (float)green_light / (float)red_light;
  apdsPlex(DIR);
  apds.isAlsAvailable();
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  float proporcao_dir = (float)green_light / (float)red_light;
  Serial.print(proporcao_esq);
  Serial.print(" / ");
  Serial.println(proporcao_dir);
}

void inVermelho(){
  apdsPlex(0);
  apds.isAlsAvailable();
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  float proporcao = (float)green_light / (float)red_light;
  Serial.print(proporcao);
  Serial.print(" / ");
  apdsPlex(1);
  apds.isAlsAvailable();
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  proporcao = (float)green_light / (float)red_light;
  Serial.println(proporcao);
}

void inAngulo(){
  Serial.print(angular(PITCH));
  Serial.print(" / ");
  Serial.print(angular(YAW));
  Serial.print(" / ");
  Serial.println(angular(ROLL));
}

void inUltra(){
  Serial.print(ultra(ESQu));
  Serial.print(" / ");
  Serial.print(ultra(FRENTEu));
  Serial.print(" / ");
  Serial.println(ultra(DIRu));
}

void inPid(){ // Valores analógicos brutos do sensor de linha (16)
  for(int i = 0; i < 16; i++){
    selectChannel(i);
    int temp = analogRead(plex_leitura); // Lê os dados do sensor
    Serial.print(temp);
    Serial.print(" ");
  }
  Serial.println();
}

void inPidAnalog(){ //V alores analógicos processados do sensor de linha,juntamente com a média analógica e o valor de diferencial 
  fazerLeitura();
  for(int i = 0; i < 16; i++){
    Serial.print(valoresAnalogicos[i]);
    Serial.print(" ");
  }
  Serial.print(media_analogico);
  Serial.print(" ");
  Serial.println(diferencial);
}

void inPidDigital(){ // Valores digitais (0 ou 1) e as médias analógica e digital para ajudar na calibração do seguidor de linha
  fazerLeitura();
  for(int i = 0; i < 16; i++){
      Serial.print(valoresDigitais[i]);
      Serial.print(" ");
  }
  Serial.print(media_digital);
  Serial.print(" ");
  Serial.println(media_analogico);
}

void inPidPesos(){ // Valores de calibração (mínimo e máximo) de cada um dos 16 sensores, que são usados para o algoritmo de pesos do PID.
  for(int i = 0; i < 16; i++){
    Serial.print(valorMaximo[i]);
    Serial.print(" ");
  }
  Serial.println();
  for(int i = 0; i < 16; i++){
    Serial.print(valorMinimo[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void inTof(){
  Serial.print(tof(ESQt));
  Serial.print(" ");
  Serial.print(tof(FRENTEt));
  Serial.print(" ");
  Serial.print(tof(DIRt));
  Serial.println(" ");
}

void imCom(){
  Serial.print(dado1);
  Serial.print(" ");
  Serial.print(dadoX);
  Serial.print(" ");
  Serial.print(dadoY);
  Serial.print(" ");
  Serial.print(dadoA);
  Serial.println(" ");
}
 
void scanI2c(){ // Verificar e listar todos os dispositivos conectados na rede I2C.
  byte error, address;
  int nDevices = 0;
  Serial.println("Procurando dispositivos I2C...");
 
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Dispositivo encontrado no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
 
  }
  if (nDevices == 0) {
    Serial.println("Nenhum dispositivo I2C encontrado.");
  } else {
    Serial.println("Varredura concluída.");
  }
  delay(5000);
}
