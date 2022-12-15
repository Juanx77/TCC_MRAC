// Arquivo modificado do código utilizado no módulo com motor tacogerador

// Definição das variáveis
long  tempo = 0; // Var. aux. pra analise no tempo discreto
float rk = 0; // Referencia tacogerador
float yk = 0; // Saída tacogerador

float e0 = 0; // Erro atual
float e1 = 0; // Erro antigo

float v0 = 0; // Sinal de controle de velocidade atual
float v1 = 0; // Sinal de controle de velocidade antigo

float Ts = 0.025; // Intervalo de analise
float Kc = 1.8573; // Ganho
float Ti = 0.72; // Tempo integral

//  KcK/Tis / (1 + KcK/Tis) = 1 (Tis/KcK s + 1)
//  taumf = Ti/KcK -> Kc = Ti/Kctaumf


void setup() {
Serial.begin(115200);
pinMode(9, OUTPUT); // Porta D9 envia sinal de controle de velocidade do motor DC, por sinal PWM
}


void loop() {
tempo = millis();

yk = analogRead(0)*5.0/1023.0; // Leitura de velocidade do motor tacogerador (de 0 a 5)
rk = 1.0 + 0.5*(int((millis()/5000)%2) == 1); // Referencia para o tacogerador variando com o tempo

e1 = e0; // Erro antigo (e1) = Erro atual (e0)
e0 = rk - yk; // Erro atual (e0) = Referencia (rk) - Saida (yk)

v1 = v0; // Sinal de controle antigo (v1) = Sinal de controle atual (v0)
v0 = v1 + Kc*e0 + (Ts*Kc/Ti - Kc)*e1; // Controlador PI
v0 = max(min(v0,5.0),0.0); // Limite superior do sinal de controle= 5, limite inferior= 0

analogWrite(9, (v0*255.0/5.0)); // Sinal PWM (0 a 255) simulando comando do controlador


// Serial print

// pronto para serial plotter
/*
Serial.print("Referencia: ");
Serial.print(rk, 4);
Serial.print(", ");
Serial.print("Saida: ");
Serial.println(yk, 4); 
*/

// mais visual
Serial.print("Referencia: ");
Serial.print(rk, 4);
Serial.print(", ");
Serial.print("Saida: ");
Serial.println(yk, 4);

Serial.print("Erro: ");
Serial.println(e0, 4);
Serial.print("Comando: ");
Serial.println(v0, 4);
   
while(millis()-tempo<=(Ts*1000)){} // Delay variável para garantir analise no tempo discreto
}