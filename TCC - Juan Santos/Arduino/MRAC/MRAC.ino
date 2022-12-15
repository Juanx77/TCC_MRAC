// Teste 1 de implementação no MRAC no modulo tacogerador
// tomando como base um modelo de primeira ordem com grau 1
// se baseando no exercício da sessao 4.5.1 do livro do Tambara
// Feito para MRAC para modelo de primeira ordem com 1 grau relativo


// FALTA:
// DESCOBRIR O MENOR PERIODO DE AMOSTRAGEM POSSIVEL;
// CRIAR UMA RAMPA DE SP MELHOR;
// TESTAR TAXAS AINDA MENORES;
// TESTAR MALHAS LENTAS E RAPIDAS;
// TENTAR DESCOBRIR OS LIMITES DE U


// Discretizaçao por Euler (s -> z)
// Y(s)/U(s) = K/(tau*s + a)        // a costuma ser 1
// tau*s*Y(s) + Y(s)*a = K*U(s)*a   // lembrando q "s" é primeira derivada
// tau*dy/dt = -y*a + K*u*a         // e sabendo q dy/dt = [y(k+1) - y(k)]/Ts
// [y(k+1) - y(k)]/Ts = (-a * y(k))/tau + (K*u(k)*a)/tau
// y(k+1) - y(k) = ((-a * y(k))/tau + (K*u(k)*a)/tau)*Ts
// y(k+1) = y(k) + ((-a * y(k))/tau + (K*u(k)*a)/tau)*Ts  
        // passando para o tempo discreto (z)
// z*Y(z) = Y(z) + ((-a * Y(z))/tau + (K*U(z)*a)/tau)*Ts
// z*Y(z) = Y(z) + (-a*Y(z)*Ts)/tau + (K*a*U(z)*Ts)/tau
// z*Y(z) - Y(z) + (a*Ts/tau) * Y(z) = (K*a*Ts/tau) * U(z)
// Y(z) * (z - 1 + a*Ts/tau) = (K*a*Ts/tau) * U(z)
// Y(z)/U(z) = (K*a*Ts/tau) / (z + (- 1 + a*Ts/tau))


// A BIBLIOTECA PRECISARÁ DE 7 ENTRADAS
// mrac.run(numM_s, denM_s[0], denM_s[1], adapt, SP, motor, taco)
// numM_s: numerador da TF do modelo no plano de Laplace (s), tbm conhecido por K
// denM_s[0]: constante que acompanha "s", tbm conhecido por tau
// denM_S[1]: constante "a" no primeiro grau da funçao
// adapt: taxa de adaptaçao do codigo (maior = mais agressivo)
// motor: porta PWM utilizada para chaveamento do motor
// taco: porta analog utilizada para leitura da tensao no tacogerador

// PENSAR EM COMO MUDAR APENAS UMA VARIAVEL PARA NAO TER Q DEFINIR TUDO O TEMPO TODO
// POR EXEMPLO SERIAL. QUE SO RECEBE O BAUDRATE UMA VEZ, DPS É APENAS OUTRAS FUNÇOES



// Portas fisicas com o módulo
#define motor 9  // motor é controlado pela porta PWM 9
#define taco A0  // tacogerador é lido pela porta analogica A0

// Modelo de referencia e coeficiente do modelo (s)
float numM_s;
float denM_s[2];
float am;

// Modelo de referencia e coeficientes do modelo (z)
float numM_z;
float denM_z[2];
float bmz;
float amz;

// Filtro (s)
float numF_s;
float denF_s[2];

// Filtro e coeficientes (z)
float numF_z;
float denF_z[2];
float beta;
float alfa;

// Referencia, saida real e do modelo atual e antigo
float r;
float r_1 = 0;
float y;
float y_1 = 0;
float ym;
float ym_1 = 0;

// Erro atual e antigo
float erro;
float erro_1 = 0;

// Ponderaçao do controlador atual e antigo, e sinal de controle
float theta1;
float theta1_1 = 0;
float theta2;
float theta2_1 = 0;
float u;

// Referencia e saída filtrados atual e antigo
float filtroR;
float filtroR_1 = 0;
float filtroY;
float filtroY_1 = 0;

// Período de amostragem e taxa de adaptação
float period;
float adapt;

// Aux para tempo de amostragem
long tempo_inicial;

// Aux do grafico
int teto = 100;
int chao = 0;


void setup() {
  Serial.begin(9600);
  pinMode(motor, OUTPUT);

  // Modelo da planta (s) e Discretização da planta (z)
  // nao é necessário informar, afinal de contas "o controlador que se vire"

  // Modelo de referencia (s)
  // M(s) = K * a/(s+a)
  numM_s = 1;     // K
  denM_s[0] = 1;  // ganho de s
  denM_s[1] = 20;  // a
  // M_s = tf(numM_s, denM_s);
  am = denM_s[1];  // Coeficiente pelo denominador do modelo

  // Discretização do modelo de referencia (z)
  period = 0.01;  // Período de amostragem (em segundos)
  // M(z) = (K*a*Ts/tau) / (z + (- 1 + a*Ts/tau))
  numM_z = numM_s * denM_s[1] * period / denM_s[0];
  denM_z[0] = 1;  // z
  denM_z[1] = -1 + denM_s[1] * period / denM_s[0];
  // M_z = tf(numM_z,  denM_z, period);
  bmz = numM_z;     // Coeficiente do numerador
  amz = denM_z[1];  // Coeficiente do denominador

  // Projeto do MRAC
  adapt = 0.00001;  // Taxa de adaptaçao

  // Filtro digital (s) para obtenção do filtroR e filtroY
  // filtroR(s) = F(s) * r(s) , ou seja, filtroR é a referencia filtrada
  // filtroY(s) = F(s) * y(s) , ou seja, filtroY é a saida filtrada
  // F(s) = am/(s + am) , ou seja, o filtro é dependente do modelo
  numF_s = am;
  denF_s[0] = 1;
  denF_s[1] = am;
  // G_filtro_s = tf(numF_s, denF_S);

  // Discretizando o filtro digital (z)
  numF_z = numF_s * denF_s[1] * period / denF_s[0];
  denF_z[0] = 1;  // z
  denF_z[1] = -1 + denF_s[1] * period / denF_s[0];
  beta = numF_z;
  alfa = denF_z[1];
  // F(z) = beta/(z + alfa)
}


void loop() {
  // Controlador MRAC de fato rodando
  // r será a referencia atual, e r_1 será a anterior
  // a mesma regra vale para todas as variaveis
  tempo_inicial = millis();

  r = 800 + 500 * (int((millis() / 5000) % 2) == 1);
  // r = 0 + 100 * 
  r = max(min(r, 1000), 0);
  ym = -amz * ym_1 + bmz * r_1;
  ym = max(min(ym, 1000), 0);

  y = analogRead(taco) * 1.4826; 
  // ganho para compensar o perca de tensao da entrada do motor à saida do taco
  // sem ganho a leitura fica proxima de 690
  // o ganho faz a media se tornar o novo valor maximo
  y = max(min(y, 1023), 0); // limita a leitura de 0 a 1023
  y = map(y, 0, 1023, 0, 1000); // converte a leitura para os calculos (TALVEZ SEJA RETIRADO)
  // y = 0 -> 0% de velocidade do motor
  // y = 1000 -> 100,0% de velocidade do motor

  theta1 = theta1_1 - period * adapt * filtroR_1 * erro_1;
  theta2 = theta2_1 + period * adapt * filtroY_1 * erro_1;

  u = theta1 * r - theta2 * y;
  // u = 0 -> 0,0% de velocidade do motor
  // u = 255 -> 100% de velocidade do motor
  u = max(min(u, 2000), 0);  // Saturaçao do sinal de controle
  u = map(u, 0, 2000, 0, 255);
  analogWrite(motor, u);
  erro = y - ym;

  // Saída dos filtros
  filtroR = -alfa * filtroR_1 + beta * r_1;
  filtroY = -alfa * filtroY_1 + beta * y_1;

  // Salvando valores atuais como antigos
  ym_1 = ym;
  r_1 = r;
  y_1 = y;
  theta1_1 = theta1;
  theta2_1 = theta2;
  erro_1 = erro;
  filtroR_1 = filtroR;
  filtroY_1 = filtroY;

  // Serial Monitor
  /*
  Serial.println("___________________________");
  Serial.print("SP: ");
  Serial.println(r);
  Serial.print("U: ");
  Serial.println(u);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("Ym: ");
  Serial.println(ym);
  Serial.println();

  Serial.print("Theta 1: ");
  Serial.println(theta1);
  Serial.print("Theta 2: ");
  Serial.println(theta2);
  Serial.print("Filtro R: ");
  Serial.println(filtroR);
  Serial.print("Filtro Y: ");
  Serial.println(filtroY);
  Serial.print("Alfa: ");
  Serial.println(alfa);
  Serial.print("Beta: ");
  Serial.println(beta);
  */

  // Serial Plotter

  //Serial.print("SP"); // Value 1
  Serial.print(r / 10);
  Serial.print("\t");
  //Serial.print("Y"); // Value 2
  Serial.print(y / 10);
  Serial.print("\t");
  //Serial.print("Ym"); // Value 3
  Serial.print(ym / 10);
  Serial.print("\t");
  //Serial.print("u"); // Value 4
  Serial.print(u/2.55);
  Serial.print("\t");

  //Serial.print("Theta 1"); // Value 5
  Serial.print(theta1);
  Serial.print("\t");
  //Serial.print("Theta 2"); // Value 6
  Serial.print(theta2);
  Serial.print("\t");


  Serial.print(teto);  // Value 7
  Serial.print("\t");
  Serial.println(chao);  // Value 8

  // delay variavel para analisar no intervalo definido (period / Ts)
  while (millis() - tempo_inicial <= period * 1) {}
}