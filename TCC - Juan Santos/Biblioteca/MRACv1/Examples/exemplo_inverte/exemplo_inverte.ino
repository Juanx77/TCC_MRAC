#include <saida(h).h>

// Cria um rele passando como parâmetro o pino no qual está ligado
 
Saida rele(8);
const byte tecla = 2;
 
// Configura arduino
 
void setup()
{  
 pinMode(tecla, INPUT);
}
 
 
// Loop principal
void loop()
{
 if (digitalRead(tecla) == LOW)
 {
   while (digitalRead(tecla)  == LOW); // Aguada tecla ser liberada
   rele.inverte();
 }
}