// Arquivo original do código utilizado no módulo com motor tacogerador

// Definição das variáveis
long  tempo = 0; // Var. aux. pra analise no tempo discreto
float rk = 0; // Referencia tacogerador
float yk = 0; // Saída tacogerador
float yf = 0;
float e0 = 0; // Erro atual
float e1 = 0; // Erro antigo
float e2 = 0;
float u0 = 0;
float u1 = 0;
float u2 = 0;
float v0 = 0; // Sinal de controle de velocidade atual
float v1 = 0; // Sinal de controle de velocidade antigo
float v2 = 0; 
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

yk = analogRead(A0)*5.0/1023.0; // Leitura de velocidade do motor tacogerador (de 0 a 5)
rk = 1.0 + 0.5*(int((millis()/5000)%2) == 1); // Referencia para o tacogerador variando com o tempo

e1 = e0; // Erro antigo (e1) = Erro atual (e0)
e0 = rk - yk; // Erro atual (e0) = Referencia (rk) - Saida (yk)

v1 = v0; // Sinal de controle antigo (v1) = Sinal de controle atual (v0)
v0 = v1 + Kc*e0 + (Ts*Kc/Ti - Kc)*e1; // Controlador
v0 = max(min(v0,5.0),0.0); // Limite superior do sinal de controle= 5, limite inferior= 0
//u0=max(min(pow(v0,2.22),1.0),0.0); // QUAL A NECESSIDADE DISSO???

//analogWrite(9, (v0*255.0/5.0)); // Sinal PWM (0 a 255) simulando comando do controlador
analogWrite(9, 255);

Serial.print(rk);
Serial.print("\t");
Serial.print(yk);
Serial.print("\t");
Serial.print(v0);
Serial.print("\t");
Serial.println(0);
//Serial.println(";");*/
/*
//Serial.print(millis()/1000.0,4);
//Serial.print(",");
Serial.print("Referencia: ");
Serial.print(rk, 4);
Serial.print(", ");
Serial.print("Saida: ");
Serial.print(yk, 4);
Serial.print(", ");
Serial.print("Controlador: ");
Serial.print(v0,4);
Serial.println(";");*/
   
while(millis()-tempo<=(Ts*1000)){} // Delay variável para garantir analise no tempo discreto
}