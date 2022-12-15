#ifndef MRAC_H
#define MRAC_H
 
#include <Arduino.h>
 
class MRAC {
public:
   MRAC(int motor, int taco);
   void  maximo();
   void  minimo();
 
private:
   int motor;
   int taco;
};
 
#endif