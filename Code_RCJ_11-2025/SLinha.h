void setup_sLinha();
void sLinha();
void sLinhaSubida();
void sLinhaDescida();
void sLinhaRampa();

uint16_t sCor(ladoSensor lado, corSensor cor); // Declaração cruzada com Verde.h

bool nLinha = false; // Aponta quando perdeu a linha
long imuGangorra = 0;

void sLinha(){
  if(sCor(ESQ, LUZ) > apds_preto_esq){
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(100, 100);
      if(infra(-1) > in_preto){
        nLinha = true;
      }
    }
    else{
      moveMotores(-200, 200);
    }
  }
  else{
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(200, -200);
    }
    else{
      moveMotores(100, 100);
    }
  }
}

void sLinhaSubida(){
  if(infra(0) > in_preto){
    if(infra(15) > in_preto){
      sLinhaRampa();
    }
    else{
      moveMotores(255, 130);
      Serial.println("sobe lento dir");
    }
  }
  else{
    if(infra(15) > in_preto){
      moveMotores(130, 255);
      Serial.println("sobe lento esq");
    }
    else{
      sLinhaRampa();
    }
  }
}

void sLinhaDescida(){
  if(sCor(ESQ, LUZ) > apds_preto_esq){
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(75, 75);
    }
    else{
      moveMotores(130, 15);
    }
  }
  else{
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(15, 130);
    }
    else{
      moveMotores(75, 75);
    }
  }
}

void sLinhaRampa(){
  if(sCor(ESQ, LUZ) > apds_preto_esq){
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(200, 200);
    }
    else{
      moveMotores(255, 50);
      Serial.println("sobe rápido dir");
      repitaPorTempo(300){
        Serial.println("atéo infinito dir");
        if(infra(-1) < in_preto){
          delay(50);
          break;
        }
      }
    }
  }
  else{
    if(sCor(DIR, LUZ) > apds_preto_dir){
      moveMotores(50, 255);
      Serial.println("sobe rápido esq");
      repitaPorTempo(300){
        Serial.println("atéo infinito esqw");
        if(infra(-1) < in_preto){
          delay(50);
          break;
        }
      }
    }
    else{
      moveMotores(200, 200);
    }
  }
}
