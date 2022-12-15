// Criado por Juan Santos

// Esse exemplo é para mostrar o uso da função plotVisual()
// onde é possível obversar dados importantes do sistema no Serial Monitor de forma clara:
// SetPoint, sinal de controle (U), Saida real, Saida do modelo, 
// theta1, theta2, filtroR, filtroY, alfa, beta

// AVISOS:
// A função plotVisual() funciona no void setup() e void loop();

#include <MRAC.h>

// Configuraçao inicial do controlador com todos os dados
DMRAC modulo1(9, 0, 1, 1, 20, 0.00001, 55);
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

  Serial.begin(9600);

  // Mostra as informações antes do sistema iniciar
  modulo1.plotVisual();

  // delay apenas para ter tempo de visualizar no Serial Monitor
  delay(2000);

  // variavel pra auxiliar no exemplo em loop
  comeco = millis();
}

void loop() {
  // Inicia o loop de calculos
  modulo1.run();

  if (5000 <= (millis() - comeco)) { // dps de 5 segundos do inicio
    modulo1.SetPoint(40); // Modifica o SP, agora durante o seu funcionamento
    
    // e se quiser, mostrar se o sistema recebeu o novo valor
    modulo1.mostraSetPoint();
  }

  // Mostra as novas informações do sistema a cada iteração
  modulo1.plotVisual();

}
