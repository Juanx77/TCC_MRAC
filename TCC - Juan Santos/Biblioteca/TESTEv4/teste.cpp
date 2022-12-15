#include"teste.h"

teste::teste(float inicio){
    Serial.print("Valor inicial: ");
    Serial.println(inicio);

    numero = inicio;
}

void teste::adiciona(float acressimo){
    Serial.print("Valor antigo: ");
    Serial.println(numero_1);

    numero = numero_1 + acressimo;

    numero_1 = numero;

    Serial.print("Valor novo: ");
    Serial.println(numero);
}

void teste::tira(float subtrai){


}