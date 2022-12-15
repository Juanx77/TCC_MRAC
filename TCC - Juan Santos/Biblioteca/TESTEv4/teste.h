#ifndef teste_H 
#define teste_H 

#include <Arduino.h>

class teste {
    public:
        teste(float inicio);

        void    adiciona(float acressimo);
        void    tira(float subtrai);

    private:
        float inicio;
        float acressimo;
        float subtrai;

        float numero_1 = 0;
};
 
#endif