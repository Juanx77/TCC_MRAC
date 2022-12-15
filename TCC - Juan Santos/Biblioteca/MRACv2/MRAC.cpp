#include"MRAC.h"
 
MRAC::MRAC(int motor, int taco)
{
   pinMode(motor, OUTPUT);
   pinMode(taco, INPUT);
}
 
void MRAC::maximo()
{
   digitalWrite(motor, HIGH);
   Serial.println("Motor no máximo com sucesso");
   int velocidade;
   velocidade = analogRead(taco);
   Serial.print("taco: ");
   Serial.println(velocidade);
}
 
void MRAC::minimo()
{
   digitalWrite(motor, LOW);
   Serial.println("Motor desligado com sucesso");
   int velocidade;
   velocidade = analogRead(taco);
   Serial.print("taco: ");
   Serial.println(velocidade);
}