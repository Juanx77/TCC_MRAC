#ifndef SAIDA(H)_H
#define SAIDA(H)_H

#include <Arduino.h>

class Saida{
public:
	Saida(int pin);
	void liga();
	void desliga();
	void inverte();

private:
	int pino;
};

#endif