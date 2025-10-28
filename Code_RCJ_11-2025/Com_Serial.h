void limpeza(); // Limpa todo o buffer serial (até 64 bytes)
void envioOn(); // Habilita envio contínuo de comandos
void envioOff(); // Desabilita envio contínuo de comandos
void receberDados(bool filtro = false); // Recebe e armazena dados do pacote

const byte header = '#'; // Header do pacote
const int tamanhoPacote = 7;   // Tamanho total do pacote (inclui header)
byte buffer[tamanhoPacote - 1]; // // Buffer de dados (4 bytes)

byte dado1; // Há objetos na imagem? (0 = não)
byte dadoX;// Cordenada X (0 - 100)
byte dadoY; // Cordenada Y (0 - 100)
byte dadoA; // Área da vítima (0 - 255)
byte dadoX1; // X mínimo
byte dadoX2; // X máximo

void limpeza(){
  while (Serial3.available()) {
    Serial3.read();
  }
}

void envioOn(char letra) {
  ledsRed();
  while (Serial3.read() != letra) {
    Serial3.write(letra);
  }
  
  limpeza();
  limpaLeds();  
  ledsGreen();
  delay(250);
  limpaLeds();
}

void envioOff(){
  Serial3.print(0);
}

void receberDados(bool filtro = false){
  limpeza();
  while (true){
    while (Serial3.available() < tamanhoPacote);
    if (Serial3.read() == header) {
      for (int i = 0; i < tamanhoPacote - 1; i++) {
        buffer[i] = Serial3.read();
      }
      dado1 = buffer[0];
      dadoX = buffer[1];
      dadoY = buffer[2];
      dadoA = buffer[3];
      dadoX1 = buffer[4];
      dadoX2 = buffer[5];

      Serial.print("Dados - ");
      Serial.print(dado1);
      Serial.print(" ");
      Serial.print(dadoX);
      Serial.print(" ");
      Serial.print(dadoY);
      Serial.print(" ");
      Serial.print(dadoA);
      Serial.print(" ");
      Serial.print(dadoX1);
      Serial.print(" ");
      Serial.print(dadoX2);
      Serial.println(" ");
      break;
    }
  }

  if (filtro){
    if (dadoX == 0 || dadoX == 100){
      dado1 = 0;
      dadoX = 0;
      dadoY = 0;
      dadoA = 0;
      dadoX1 = 0;
      dadoX2 = 0;
    }
  }
}
