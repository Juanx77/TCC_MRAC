// Criado por Juan Santos

// Esse exemplo é para mostrar o uso geral desta biblioteca
// recomenda-se ler antes os outros exemplos

#include <MRAC.h>

//  Esse exemplo mostra como usar a funçao run da biblioteca MRAC

// Configuraçao inicial do controlador com todos os dados
DMRAC modulo1(9, 0, 4, 2, 10, 2, 65);
  // motor: porta PWM que vai sair o sinal de controle para o motor 1
  // taco: porta analog que vai ler a tensao do motor 2
  // M_num: ganho da Funçao de Transferencia modelo
  // M_den0: primeira constante do denominador da Funçao de Transferencia modelo
  // M_den1: segunda constante do denominador da Funçao de Transferencia modelo
  // adapt: taxa de adaptaçao do MRAC
  // sp: SetPoint da velocidade em porcentagem

// variavel pra auxiliar no exemplo em loop
long comeco;


void setup() {
  // Tempo de espera para garantir que o Arduino esteja completamente inicializado
  delay(2000);

  Serial.begin(9600);

  modulo1.ModeloGanho(5);
  modulo1.mostraModeloGanho();
  delay(1000);

  modulo1.ModeloDenominador(3, 5);
  modulo1.mostraModeloDenominador();
  delay(1000);

  modulo1.Modelo(1, 1, 20);
  modulo1.mostraModelo();
  delay(1000);

  modulo1.Adaptacao(0.00001);
  modulo1.mostraAdaptacao();
  delay(1000);

  modulo1.SetPoint(0);
  modulo1.mostraSetPoint();
  delay(1000);

  comeco = millis();
}


void loop() {
  // Inicia o loop de calculos do MRAC
  modulo1.run();

  modulo1.plotGrafico();
  //modulo1.plotVisual();

  if ( 5000 <= (millis() - comeco) and (millis() - comeco) < 10000 ) { // dps de 5 segundos do inicio
    modulo1.SetPoint(10);
  }

  else if ( 10000 <= (millis() - comeco) and (millis() - comeco) < 15000 ){ // dps de 10 segundos do inicio
    modulo1.SetPoint(20);
  }

  else if ( 15000 <= (millis() - comeco) and (millis() - comeco) < 20000 ){ // dps de 15 segundos do inicio
    modulo1.SetPoint(30);
  }

  else if ( 20000 <= (millis() - comeco) and (millis() - comeco) < 25000 ){ // dps de 20 segundos do inicio
    modulo1.SetPoint(40);
  }

  else if ( 25000 <= (millis() - comeco) and (millis() - comeco) < 30000 ){ // dps de 25 segundos do inicio
    modulo1.SetPoint(50);
  }

  else if ( 30000 <= (millis() - comeco) and (millis() - comeco) < 35000 ){ // dps de 30 segundos do inicio
    modulo1.SetPoint(60);
  }

  else if ( 35000 <= (millis() - comeco) and (millis() - comeco) < 40000 ){ // dps de 35 segundos do inicio
    modulo1.SetPoint(70);
  }

  else if ( 40000 <= (millis() - comeco) and (millis() - comeco) < 45000 ){ // dps de 40 segundos do inicio
    modulo1.SetPoint(80);
  }

  else if ( 45000 <= (millis() - comeco) and (millis() - comeco) < 50000 ){ // dps de 45 segundos do inicio
    modulo1.SetPoint(90);
  }

  else if ( 50000 <= (millis() - comeco) and (millis() - comeco) < 55000 ){ // dps de 50 segundos do inicio
    modulo1.SetPoint(100);
  }
}