#define envioOn
#define receberDados

// Sensor de ângulo está definido no "Movimentação.h"
// Sensor de cor está definido no "Verde.h"
// Sensor de infravermelho está definido no "SLinha.h"
// Sensor ultrassônico está definido no "Códigos_Genéricos.h"

//Organização de sinalização por leds:
// 1 led azul respectivo ao lado - desviador de obstáculo
// 1 led rosa(azul/vermelho) respectivo ao lado - caidoEsq e caidoDir no código de rampa
// 2 leds azuis - gangorra
// 2 leds rosas - downgorra
// 1 led vermelho respectivo ao lado - curva de 90
// 1 ou 2 leds verdes respectivos aos lados - verde
// 2 leds vermelhos - Final da pista / Calibração / Correção PID
// 2 leds amarelos(vermelho/verde) - Perdeu linha / Calibração
// 2 leds cianos(verde/azul) - Está preso (fator I)
//

//V90 - Código de retroalimentação entre o verde e curva de 90
//e a unica função não padronizada do código

//Convenção - Esquerda quase sempre antes da direita

//Millis é uma função - [millis()]
//Enum é tipo definir um certo termo como um int para o resto do código
//O tipo union permite "linkar" o valor de duas variaveis, mesmo de tipos diferentes.

//Conversão de valores de velocidade
//60 - 150
//50 - 130
//70 - 180
//80 - 200
//85 - 215
//30 - 75
//05 - 15
//20 - 50
//18 - 45
