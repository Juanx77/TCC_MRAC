#include"saida(h).h"

Saida::Saida(int pin){
	pinMode(pin, OUTPUT);
	pino = pin;
}

void Saida::liga(){
	digitalWrite(pino, HIGH);
}

void Saida::desliga(){
	digitalWrite(pino, LOW);
}

void Saida::inverte(){
	digitalWrite(pino, !digitalRead(pino));
}