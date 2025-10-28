void limpaLeds();

void ledsGreen();
void ledsRed();
void ledsBlue();
void ledsYellow();
void ledsMagenta();
void ledsCyan();
void ledsWhite();

void piscarGreen(int numero, int tempo);
void piscarRed(int numero, int tempo);
void piscarBlue(int numero, int tempo);
void piscarYellow(int numero, int tempo);
void piscarMagenta(int numero, int tempo);
void piscarCyan(int numero, int tempo);
void piscarWhite(int numero, int tempo);

void limpaLeds(){
  digitalWrite(led_esq_R, LOW);
  digitalWrite(led_esq_G, LOW);
  digitalWrite(led_esq_B, LOW);
  digitalWrite(led_dir_R, LOW);
  digitalWrite(led_dir_G, LOW);
  digitalWrite(led_dir_B, LOW);
}

void ledsRed() {
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
}

void ledsGreen() {
  limpaLeds();
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
}

void ledsBlue() {
  limpaLeds();
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
}

void ledsYellow() {
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
}

void ledsMagenta() {
  limpaLeds();
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
}

void ledsCyan() {
  limpaLeds();
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
}

void ledsWhite() {
  limpaLeds();
  digitalWrite(led_esq_R, HIGH);
  digitalWrite(led_dir_R, HIGH);
  digitalWrite(led_esq_G, HIGH);
  digitalWrite(led_dir_G, HIGH);
  digitalWrite(led_esq_B, HIGH);
  digitalWrite(led_dir_B, HIGH);
}

void piscarGreen(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsGreen();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}

void piscarRed(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsRed();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}

void piscarBlue(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsBlue();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}

void piscarYellow(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsYellow();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}

void piscarMagenta(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsMagenta();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}

void piscarCyan(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsCyan();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}

void piscarWhite(int numero, int tempo) {
  for (int i = 0; i < numero; i++) {
    ledsWhite();
    delay(tempo);
    limpaLeds();
    delay(tempo);
  }
}
