#ifndef MRAC_H 
#define MRAC_H // Essas dieretivas nao deixarao as declarações/definições
               // da biblioteca serem inseridas mais de uma vez em um projeto
               // SEJA LA OQ ISSO SIGNIFICA
 
#include <Arduino.h> // usa pra poder ter acesso as bibliotecas do arduino
 
class DMRAC { // Direct Model Reference Adaptive Control
// cria uma classe dentro da biblioteca MRAC.h
public:
    DMRAC(int motor, int taco, float M_num, float M_den0, float M_den1, double adapt, float sp);
    // Na classe sao informadas todas as informaçoes incialmente

    // motor: porta PWM que vai sair o sinal de controle para o motor 1
    // taco: porta analog que vai ler a tensao do motor 2
    // M_num: ganho da Funçao de Transferencia modelo
    // M_den0: primeira constante do denominador da Funçao de Transferencia modelo
    // M_den1: segunda constante do denominador da Funçao de Transferencia modelo
    // adapt: taxa de adaptaçao do MRAC
    // sp: SetPoint da velocidade em porcentagem

    // Cada função dentro da classe pode mudar alguma de suas informaçoes
    void    ModeloGanho(float M_num);
    void    ModeloDenominador(float M_den0, float M_den1);
    void    Modelo(float M_num, float M_den0, float M_den1);
    void    Adaptacao(double adapt);
    void    SetPoint(float sp);
 
    // Funçoes para Setup, Loop e apresentaçao de dados
    void    start(float M_num, float M_den0, float M_den1, double adapta, float sp);
    void    run();
    void    plotVisual();
    void    plotGrafico();

    void    mostraModeloGanho();
    void    mostraModeloDenominador();
    void    mostraModelo();
    void    mostraAdaptacao();
    void    mostraSetPoint();

    void    erroIAE();
    void    erroISE();
    void    erroITAE();
    void    erroITSE();
 
private:    // evita que esses nomes atrapalhem no codigo onde a bib for usada
    // variaveis de entrada
    int     motor;
    int     taco;
    float   M_num;
    float   M_den0;
    float   M_den1;
    double  adapta;
    float   sp;

    // Modelo de referencia e coeficiente do modelo (s)
    float   numM_s;
    float   denM1_s;
    float   denM2_s;
    float   am;

    // Modelo de referencia e coeficientes do modelo (z)
    float   numM_z;
    float   denM1_z;
    float   denM2_z;
    float   bmz;
    float   amz;

    // Filtro (s)
    float   numF_s;
    float   denF1_s;
    float   denF2_s;

    // Filtro e coeficientes (z)
    float   numF_z;
    float   denF1_z;
    float   denF2_z;
    float   beta;
    float   alfa;

    // Referencia, saida real e do modelo atual e antigo
    float   r;
    float   r_1 = 0;
    float   y;
    float   y_1 = 0;
    float   ym;
    float   ym_1 = 0;

    // Erro atual e antigo
    float   erro;
    float   erro_1 = 0;

    // Ponderaçao do controlador atual e antigo, e sinal de controle
    float   theta1;
    float   theta1_1 = 0;
    float   theta2;
    float   theta2_1 = 0;
    float   u;

    // Referencia e saída filtrados atual e antigo
    float   filtroR;
    float   filtroR_1 = 0;
    float   filtroY;
    float   filtroY_1 = 0;

    // Período de amostragem e taxa de adaptação
    float   period;
    float   adapt;

    // Aux para tempo de amostragem
    long    tempo_inicial;

    // Aux do grafico
    int     teto = 100;
    int     chao = 0;

    // testes
    float   resposta;
    int     motorP;
    int     tacoP;
    float   IAE;
    float   ISE;
    float   ITAE;
    float   ITSE;
    long    tempo;
};
 
#endif