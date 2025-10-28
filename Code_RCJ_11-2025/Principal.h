void subLoop();

long nPreto = 0; // Contador de tempo sem ver a linha (milissegundos)
long nPreto_old = 0;  // Armazena o millis() do último momento em que a linha foi vista
bool nLinhaInicio = false; // Flag para marcar o início de um período sem linha

void subLoop(){
  resetPid();
  if(ehVermelho(ESQ) && ehVermelho(DIR)){ // Achou vermelho
    fim();
  }
  else if(analogRead(prata_pin) > prata){
    salaDeResgate();
  }
  else if(ultra(FRENTEu) <= 2){ //
    Stop();
    if(media(ultra, FRENTEu, 5 <= 6)){
      desviaDir();
    }
  }
  else if((angular(PITCH) > 8 | angular(PITCH) < -8) && millis() >= imuGangorra){ // Se está descendo ou subindo e não possui imunidade ativa
    if(angular(PITCH) < -8){
      gangorra();
    }
    else {
      downgorra();
    }
  }
  else if(nPreto > 300 && imuRec < millis()){ // Robô não viu a linha por mais de 150ms e o tempo de imunidade da recuperação já passou // 150
    recLinha();
    nPreto = 0; // Reseta o tempo sem ver a linha
  }
  else {
    verde90();
  }
  if(!comI){
    tempo_erro = 0;
  }
  comI = false;
  
/*
  if((angular(PITCH)<-7||angular(PITCH)>7||angular(ROLL)<-12||angular(ROLL)>12)&&millis()>imuGangorra){ // Se está inclinado e sem imunidade
    //rampa();
  }
*/
  
  if(nLinha){ // Robô não está lendo linha. Reseta nPreto se não passou reto pelo seguidor vendo só branco
    if(nLinhaInicio){
      nPreto_old = millis(); // Última vez que vi a linha
      nLinhaInicio = false;
    }
    nPreto = millis() - nPreto_old;
  } else{ // Robô não está lendo linha
    nLinhaInicio = true;
  }
  nLinha = false;
}
