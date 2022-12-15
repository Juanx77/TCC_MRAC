% Exerc�cio da se�ao 4.5.1  - Planta de primeira ordem
% Controlador MRAC aplicado a uma planta de primeira ordem

clc
clear variables
close all
format long e

% Sinal de referencia (r)
Amp = 1; % amplitude de referencia
Te = 200; % Dura��o do sinal de referencia

%% Modelo da planta (s)
num_s = 0.8;
den_s = [0.6 1];
G_s = tf(num_s, den_s)

% Discretização da planta
T = 0.01; % Período de amostragem (em segundos)
[num_z, den_z] = c2dm(num_s, den_s, T, 'zoh');
G_z = tf(num_z, den_z, T)
bz = num_z(2); % Coeficiente do numerador
az = den_z(2); % Coeficiente do denominador

%% Modelo de referencia (s)
numM_s = 5;
denM_s = [1 5];
M_s = tf(numM_s, denM_s)
am = denM_s(2); % Coeficiente do denominador do modelo

% Discretização do modelo de referencia
[numM_z, denM_z] = c2dm(numM_s, denM_s, T, 'zoh');
M_z = tf(numM_z, denM_z, T)
bmz = numM_z(2); % Coeficiente do numerador
teste = numM_z(1);
amz = denM_z(2); % Coeficiente do denominador

%% Reserva de espaço para os vetores de interesse
ntotal = 1e5; % Numero total de pontos no gráfico
r = zeros(1, ntotal);
ym = zeros(1, ntotal);
y = zeros(1, ntotal);
u = zeros(1, ntotal);
e1 = zeros(1, ntotal);
tempo = zeros(1, ntotal);

theta1 = zeros(1, ntotal); % Ganho 1 do controlador
theta2 = zeros(1, ntotal); % Ganho 2 do controlador

zetaR = zeros(1, ntotal); % Referencia filtrada por am/(s+am)
zetaY = zeros(1, ntotal); % Saida filtrada por am/(s+am)

%% Projeto do MRC
theta1_est = bmz/bz
theta2_est = (amz - az)/bz

%theta1_est = 3.98;
%theta2_est = 1.98;

% Taxa de adaptação do mecanismo de adaptação de ganhos
g = 10;

%% Filtro digital para obtenção do zetaR e do zetaY
%zetaR(s) = F(s) * r(s)
%zetaY(s) = F(s) * y(s)
%F(s) = am/(s + am)
num2 = am;
den2 = [1 am];
Gf_s = tf(num2, den2)

% Discretizando
[num2_z, den2_z] = c2dm(num2, den2, T, 'zoh');
Gf_z = tf(num2_z, den2_z)
beta = num2_z(2)
alfa = den2_z(2)
%obtem-se
%zetaR(z) = F(z) * r(z)
%zetaY(z) = F(z) * y(z)
%F(z) = beta/(z + alfa)

%% Laço de repetição para execução do controlador MRAC
for k = 2:ntotal
  r(k) = Amp * square(2*pi*k*T/Te);
  ym(k) = -amz*ym(k-1) + bmz*r(k-1);
  y(k) = -az*y(k-1) + bz*u(k-1) - 0.02*(k>ntotal/2);

  theta1(k) = theta1(k-1) - T*g*zetaR(k-1)*e1(k-1);
  theta2(k) = theta2(k-1) + T*g*zetaY(k-1)*e1(k-1);

  u(k) = theta1(k)*r(k) - theta2(k)*y(k);

  e1(k) = y(k) - ym(k);

  % Saídas dos filtros
  zetaR(k) = -alfa*zetaR(k-1) + beta*r(k-1);
  zetaY(k) = -alfa*zetaY(k-1) + beta*y(k-1);

  tempo(k) = (k-2)*T;
end

plot(tempo, r, tempo, y);
figure(2)
plot(tempo, theta1, tempo, theta2)