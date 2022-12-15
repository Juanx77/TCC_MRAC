#include <teste.h>

teste tentar(3); // valor inicial

void setup() {
  delay(2000);
  Serial.begin(9600);
}

void loop() {
  tentar.adiciona(5);
  delay(2000);
  tentar.tira(1);
  delay(2000);
}
