#include"MRAC.h"

DMRAC::DMRAC(int motor, int taco, float M_num, float M_den0, float M_den1, double adapta, float sp) {
    //pinMode(motor, OUTPUT);
    //pinMode(taco, INPUT);

    motorP = motor;
    tacoP = taco;

    pinMode(motorP, OUTPUT);
    pinMode(tacoP, INPUT);

    DMRAC::start(M_num, M_den0, M_den1, adapta, sp); // AQUI VAI UMA FUNÇAO QUE FAZ TODO O SETUP pela primeira vez
}

// Funçao para modificar o ganho do Modelo (numM_s)
void DMRAC::ModeloGanho(float M_num) {  
    // M(s) = K * a/(s+a)
    numM_s = M_num;

    DMRAC::start(M_num, M_den0, M_den1, adapta, sp); // AQUI VAI UMA FUNÇAO QUE FAZ TODO O SETUP DNV
}

void DMRAC::mostraModeloGanho(){
    Serial.println();
    Serial.print("Novo ganho do modelo eh: ");
    Serial.println(numM_s);
}

// Funçao para modificar as constantes do Modelo (denM_s[0] e denM_s[1])
void DMRAC::ModeloDenominador(float M_den0, float M_den1) {
    // M(s) = K * a/(s+a)
    denM1_s = M_den0;
    denM2_s = M_den1;

    DMRAC::start(M_num, M_den0, M_den1, adapta, sp); // AQUI VAI UMA FUNÇAO QUE FAZ TODO O SETUP DNV
}

void DMRAC::mostraModeloDenominador(){
    Serial.println();
    Serial.print("Novas constantes do modelo sao: ");
    Serial.print(denM1_s);
    Serial.print(" e ");
    Serial.println(denM2_s);
}

// Funçao para modificar todo o Modelo (numM_s, denM_s[0] e denM_s[1])
void DMRAC::Modelo(float M_num, float M_den0, float M_den1) {
    // M(s) = K * a/(s+a)
    numM_s = M_num;
    denM1_s = M_den0;
    denM2_s = M_den1;

    DMRAC::start(M_num, M_den0, M_den1, adapta, sp); // AQUI VAI UMA FUNÇAO QUE FAZ TODO O SETUP DNV
}

void DMRAC::mostraModelo(){
    Serial.println();
    Serial.println("M(s) = K * a/(s+a)");
    Serial.print("Novo ganho do modelo eh: ");
    Serial.println(numM_s);
    Serial.print("Novas constantes do modelo sao: ");
    Serial.print(denM1_s);
    Serial.print(" e ");
    Serial.println(denM2_s);
}

// Funçao para modificar a Taxa de adaptação do MRAC
void DMRAC::Adaptacao(double adapta) {
    adapt = adapta;
    // COMO O ADAPT NAO AFETA NO SETUP, NAO PRECISA RODAR DNV O SETUP
}

void DMRAC::mostraAdaptacao() {
    Serial.println();
    Serial.print("Nova taxa de adaptacao do controlador eh: ");
    Serial.println(adapt, 6);
    Serial.println();
}

// Funçao para modificar o SetPoint
void DMRAC::SetPoint(float sp) {
    r = sp;
    // COMO O ADAPT NAO AFETA NO SETUP, NAO PRECISA RODAR DNV O SETUP
}

void DMRAC::mostraSetPoint() {
    Serial.println();
    Serial.print("Novo SP do sistema eh: ");
    Serial.println(r);
}

// Função que faz todo o Setup dnv
void DMRAC::start(float M_num, float M_den0, float M_den1, double adapta, float sp) {
    //Serial.println();
    // Modelo da planta (s) e Discretização da planta (z)
    // nao é necessário informar, afinal de contas "o controlador que se vire"

    // Modelo de referencia (s)
    // M(s) = K * a/(s+a)
    // nome usado no codigo de fato = nome de entrada
    numM_s = M_num; // K
    denM1_s = M_den0; // ganho de s
    denM2_s = M_den1; // a
    // M_s = tf(numM_s, denM_s);
    am = denM2_s;  // Coeficiente pelo denominador do modelo

    // Discretização do modelo de referencia (z)
    period = 0.01;  // Período de amostragem (em segundos) [FALTA RECALCULAR ISSO!!!]
    // M(z) = (K*a*Ts/tau) / (z + (- 1 + a*Ts/tau))
    numM_z = numM_s * denM2_s * period / denM1_s;
    denM1_z = 1;  // z
    denM2_z = -1 + denM2_s * period / denM1_s;
    // M_z = tf(numM_z,  denM_z, period);
    bmz = numM_z;     // Coeficiente do numerador
    amz = denM2_z;  // Coeficiente do denominador

    // Projeto do MRAC
    adapt = adapta; // Taxa de adaptaçao
    r = sp; // Setpoint

    // Filtro digital (s) para obtenção do filtroR e filtroY
    // filtroR(s) = F(s) * r(s) , ou seja, filtroR é a referencia filtrada
    // filtroY(s) = F(s) * y(s) , ou seja, filtroY é a saida filtrada
    // F(s) = am/(s + am) , ou seja, o filtro é dependente do modelo
    numF_s = am;
    denF1_s = 1;
    denF2_s = am;
    // G_filtro_s = tf(numF_s, denF_S);

    // Discretizando o filtro digital (z)
    numF_z = ((numF_s * denF2_s * period) / denF1_s);
    denF1_z = 1;  // z
    denF2_z = -1 + denF2_s * period / denF1_s;
    beta = numF_z;
    alfa = denF2_z;
    // F(z) = beta/(z + alfa)

    tempo = millis();

    //Serial.println("Dados atualizados!");
}

// Função que fica no Loop do código
void DMRAC::run() {
    // Controlador MRAC de fato rodando
    // r será a referencia atual, e r_1 será a anterior
    // a mesma regra vale para todas as variaveis
    tempo_inicial = millis();

    ym = -amz * ym_1 + bmz * 10 * r_1;
    ym = max(min(ym, 1000), 0);

    y = analogRead(tacoP) * 1.705; 
    // ganho para compensar o perca de tensao da entrada do motor à saida do taco
    // sem ganho a leitura fica proxima de 690
    // o ganho faz a media se tornar o novo valor maximo
    y = max(min(y, 1023), 0); // limita a leitura de 0 a 1023
    y = map(y, 0, 1023, 0, 1000); // converte a leitura para os calculos (TALVEZ SEJA RETIRADO)
    // y = 0 -> 0% de velocidade do motor
    // y = 1000 -> 100,0% de velocidade do motor

    theta1 = theta1_1 - period * adapt * filtroR_1 * erro_1;
    theta2 = theta2_1 + period * adapt * filtroY_1 * erro_1;

    u = theta1 * 10 * r - theta2 * y;
    // u = 0 -> 0,0% de velocidade do motor
    // u = 255 -> 100% de velocidade do motor
    u = max(min(u, 2000), 0);  // Saturaçao do sinal de controle
    u = map(u, 0, 2000, 0, 255);
    analogWrite(motorP, u);
    erro = y - ym;

    // Saída dos filtros
    filtroR = -alfa * filtroR_1 + beta * 10 * r_1;
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

    // Cálculo dos indices de performance
    DMRAC::erroIAE();
    DMRAC::erroISE();
    DMRAC::erroITAE();
    DMRAC::erroITSE();

    // delay variavel para analisar no intervalo definido (period / Ts)
    while (millis() - tempo_inicial <= period * 1) {}
}

void DMRAC::erroIAE() {
    IAE += abs(erro);
}

void DMRAC::erroISE() {
    ISE += abs(erro*erro);
}

void DMRAC::erroITAE() {
    ITAE += abs(erro) * tempo/1000;
}

void DMRAC::erroITSE() {
    ITSE += abs(erro*erro) * tempo/1000;
}



void DMRAC::plotVisual() {
    Serial.println("___________________________");
    Serial.println();
    Serial.print("SP: ");
    Serial.println(r);
    Serial.print("U: ");
    Serial.println(u / 2.55);
    Serial.print("Y: ");
    Serial.println(y / 10);
    Serial.print("Ym: ");
    Serial.println(ym / 10);

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

    Serial.println();

    Serial.print("motor na porta: ");
    Serial.println(motorP);
    Serial.print("taco na porta: ");
    Serial.println(tacoP);

}

void DMRAC::plotGrafico() {
    //Serial.print("SP"); // Value 1
    Serial.print(r);
    Serial.print("\t");
    //Serial.print("Y"); // Value 2
    Serial.print(y / 10);
    Serial.print("\t");
    //Serial.print("Ym"); // Value 3
    Serial.print(ym / 10);
    Serial.print("\t");
    //Serial.print("u"); // Value 4
    Serial.print(u / 2.55);
    Serial.print("\t");

    //Serial.print("Theta 1"); // Value 5
    Serial.print(theta1);
    Serial.print("\t");
    //Serial.print("Theta 2"); // Value 6
    Serial.print(theta2);
    Serial.print("\t");

    Serial.print(teto);  // Value 7
    Serial.print("\t");
    Serial.print(chao);  // Value 8
    Serial.println("\t");

    //Serial.print(IAE); // Value 9
    //Serial.print("\t");
    //Serial.print(ISE);  // Value 10
    //Serial.print("\t");
    //Serial.print(ITAE);  // Value 11
    //Serial.print("\t");
    //Serial.println(ITSE);  // Value 12
}