// Criado por Juan Santos

// Esse exemplo é para mostrar o uso das funções ModeloGanho() e mostraModeloGanho()
// onde é possível modificar o ganho da Função de Transferência modelo utilizada.
// Lembrando que a Função de Transferência do modelo se dá por:
// M(s) = K * a/(s+a)
// ou seja, essa função modifica apenas o valor de K.

// AVISOS:
// Tanto ModeloGanho() como mostraModeloGanho() funcionam no void setup() e void loop();
// Não é recomendando o uso de mostraModeloGanho() no void loop() caso esteja sendo utilizado o Serial Plotter;


#include <MRAC.h>

// Configuraçao inicial do controlador com todos os dados
DMRAC modulo1(9, 0, 5, 1, 20, 0.00001, 55);
  // motor: porta PWM que vai sair o sinal de controle para o motor 1
  // taco: porta analog que vai ler a tensao do motor 2
  // M_num: ganho da Função de Transferência modelo
  // M_den0: primeira constante do denominador da Funçao de Transferencia modelo
  // M_den1: segunda constante do denominador da Funçao de Transferencia modelo
  // adapt: taxa de adaptaçao do MRAC
  // sp: SetPoint da velocidade em porcentagem

// variavel pra auxiliar no exemplo em loop
long comeco;


void setup() {
  // Tempo de espera para garantir que o Arduino esteja completamente inicializado
  delay(2000); 

  // Pode ser removido se não for usar a função mostraModeloGanho()
  Serial.begin(9600);

  // Voce pode modificar o ganho do modelo antes do loop iniciar
  modulo1.ModeloGanho(2);
  // e se quiser, mostrar se o sistema recebeu o novo valor
  modulo1.mostraModeloGanho();

  // delay apenas para ter tempo de visualizar no Serial Monitor
  delay(2000);

  // variavel pra auxiliar no exemplo em loop
  comeco = millis();
}

void loop() {
  // Inicia o loop de calculos
  modulo1.run();

  if (5000 <= (millis() - comeco)) { // dps de 5 segundos do inicio
    modulo1.ModeloGanho(1); // Modifica novamente o ganho do modelo, agora durante o seu funcionamento
    
    // e se quiser, mostrar se o sistema recebeu o novo valor
    modulo1.mostraModeloGanho();
  }

}
