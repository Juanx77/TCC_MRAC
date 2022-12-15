#include <saida(h).h>

// Instancia um objeto chamado LED no pino 13
Saida LED(13);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  LED.liga();
  delay(1000);
  LED.desliga();
  delay(1000);

}
