% Revis�o de filtros digitais e solu��o num�rica de sistemas dinamicos

clc 
clear variables
close all
format long e

% Parametros do circuito el�trico LRC 
R = 100;
L = 1e-3;
C = 25e-6;

% Fun�ao de transferencia continua (em S)
num_s = 1/(L*C);
den_s = [1 1/(R*C) 1/(L*C)];
G_s = tf(num_s, den_s) % modelo da planta em TF em S

% Discretiza��o da planta (em Z)
T = 1e-6; % tempo de amostragem em segundos
[num_z, den_z] = c2dm(num_s, den_s, T, 'tustin');
G_z = tf(num_z, den_z, T)

% Coeficientes da planta discreta -> G(z)
% zeros da FT(z)
b0 = num_z(1);
b1 = num_z(2);
b2 = num_z(3);

% polos da FT(z)
a0 = den_z(1);
a1 = den_z(2);
a2 = den_z(3);

% Matriz e vetor do modelo em Espa�o de Estados
A = [0 -1/L; 1/C -1/(R*C)];
B = [1/L; 0];
    % Discretiza��o por Euler
    Ad = (eye(2) + A*T);
    Bd = B*T;

% Reserva de mem�ria para os vetores de interesse
ntotal = 30e3; % numero total de pontos no grafico
y = zeros(1, ntotal);
u = ones(1, ntotal);
tempo1 = zeros(1, ntotal);
tempo2 = zeros(1, ntotal);
X = zeros(2, ntotal);
% AINDA NAO ENTENDI ESSA PARTE   
 
% La�o de repeti��o da equa�ao de recorrencia
for k = 3:ntotal
   u(k) = 1;
   y(k) = b0*u(k) + b1*u(k-1) + b2*u(k-2) - a1*y(k-1) - a2*y(k-2);
   tempo1(k) = (k-3)*T;
end
% AINDA NAO ENTENDI ESSA PARTE  

% Solu��o da equa�ao de estado via m�todo de Euler
for k = 1:ntotal
    X(:,k+1) = Ad*X(:,k) + Bd*u(k);
    tempo2(k+1) = (k)*T;
end