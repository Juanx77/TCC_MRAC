% Exercício da seçao 4.5.1  - Planta de primeira ordem
% Controlador MRC (sem adaptaçao) aplicado a uma planta de primeira ordem

clc
clear variables
close all
format long e

% Sinal de referencia (r)
Amp = 1; % amplitude de referencia
Te = 20; % período fundamental da referencia

%% Modelo da planta em S
num_s = 0.5;
den_s = [1 1];
G_s = tf(num_s, den_s)
b = num_s(1); % coeficiente do numerador
a = den_s(2); % coeficiente do denominador

% Discretizaçao da planta
T = 0.01; % periodo de amostragem (em segundos)
[num_z, den_z] = c2dm(num_s, den_s, T, 'zoh');
G_z = tf(num_z, den_z, T)
bz = num_z(2); % coeficiente do numerador
az = den_z(2); % coeficiente do denominador

%% Modelo de referencia em S
numM_s = 2; % 
denM_s = [1 1];
M_s = tf(numM_s, denM_s)
bm = numM_s(1); % coeficiente do numerador
am = denM_s(2); % coeficiente do denominador

% Discretizaçao do modelo de referencia
[numM_z, denM_z] = c2dm(numM_s, denM_s, T, 'zoh');
M_z = tf(numM_z, denM_z, T)
bmz = numM_z(2); % coeficiente do numerador
amz = denM_z(2); % coeficiente do denominador

%% Projeto do MRC continuo
theta1_est_s = bm/b
theta2_est_s = (am - a)/b

% Projeto do MRC discreto (utilizado na analise)
theta1_est_z = bmz/bz
theta2_est_z = (amz - az)/bz

% tinha q dar theta1 = 4 e theta2 = 2,
% mas nesse codigo da 4 e 0

%% Reserva de espaço para os vetores de interesse
ntotal = 1e4; % numero total de pontos no grafico
r = zeros(1, ntotal);
ym = zeros(1, ntotal);
y = zeros(1, ntotal);
u = zeros(1, ntotal);
e1 = zeros(1, ntotal);
tempo = zeros(1, ntotal);

% Laço de repetiçao para execuçao do controlado MRC
for k = 2:ntotal
    r(k) = Amp*square(2*pi*k*T/Te); % referencia R
    ym(k) = -amz*ym(k-1) + bmz*r(k-1); % saida do modelo de referencia a ser seguido
    y(k) = -az*y(k-1) + bz*u(k-1); % resposta da planta
    u(k) = theta1_est_z*r(k) - theta2_est_z*y(k); % sinal de controle para a planta
    
    e1(k) = y(k) - ym(k); % erro de rastreamento
    tempo(k) = (k-2)*T; % vetor tempo (??????)
end