#include <MRAC.h>

//  Esse exemplo mostra como usar a funçao run da biblioteca MRAC

// Configuraçao inicial do controlador com todos os dados
DMRAC modulo1(9, A0, 1, 1, 20, 0.00001, 65);
  // motor: porta PWM que vai sair o sinal de controle para o motor 1
  // taco: porta analog que vai ler a tensao do motor 2
  // M_num: constante do numerador da Funçao de Transferencia modelo
  // M_den0: primeira constante do denominador da Funçao de Transferencia modelo
  // M_den1: segunda constante do denominador da Funçao de Transferencia modelo
  // adapt: taxa de adaptaçao do MRAC
  // sp: SetPoint da velocidade em porcentagem

long comeco;

void setup() {
  delay(2000);
  Serial.begin(9600);

  comeco = millis();
}

void loop() {
  modulo1.run();

  modulo1.plotGrafico();
  //modulo1.plotVisual();

  if ( 2000 <= (millis() - comeco) and (millis() - comeco) < 4000 ) { // dps de 2 segundos do inicio
    modulo1.SetPoint(100);
  }

  else if ( 4000 <= (millis() - comeco) and (millis() - comeco) < 6000 ){ // dps de 4 segundos do inicio
    modulo1.SetPoint(90);
  }

  else if ( 6000 <= (millis() - comeco) and (millis() - comeco) < 8000 ){ // dps de 6 segundos do inicio
    modulo1.SetPoint(80);
  }

  else if ( 8000 <= (millis() - comeco) and (millis() - comeco) < 10000 ){ // dps de 8 segundos do inicio
    modulo1.SetPoint(70);
  }

  else if ( 10000 <= (millis() - comeco) and (millis() - comeco) < 12000 ){ // dps de 10 segundos do inicio
    modulo1.SetPoint(60);
  }

  else if ( 12000 <= (millis() - comeco) and (millis() - comeco) < 14000 ){ // dps de 12 segundos do inicio
    modulo1.SetPoint(50);
  }

  else if ( 14000 <= (millis() - comeco) and (millis() - comeco) < 16000 ){ // dps de 14 segundos do inicio
    modulo1.SetPoint(40);
  }

  else if ( 16000 <= (millis() - comeco) and (millis() - comeco) < 18000 ){ // dps de 16 segundos do inicio
    modulo1.SetPoint(30);
  }

  else if ( 18000 <= (millis() - comeco)){                                  // dps de 18 segundos do inicio reseta
    comeco = millis();
  }

}
