void salaDeResgate();
void imCom();

void salaDeResgate(){
  Stop();
  if (!devoSair){
    setupResgate();

    if (tof(DIRt) < 150){
      procurarVitimas(-150);
      procurarVitimas(150);
    } else {
      procurarVitimas(150);
      procurarVitimas(-150);
    }

    procuraExtra("Parado", 0);
    procuraExtra("Tras", 600);
    procuraExtra("Frente", 1200);
    procuraExtra("Tras", 600);
    
    setupTriangulo();
      
    procurarTriangulo(3);
    procurarTriangulo(4);
  }
  procurarSaida();
}
