/**
 *Implementaci�n de los m�todos de la clase TypeF
 */
#include "TypeF.hpp"

using namespace simulator;

//Almacena los �ndices de los v�rtices de un tri�ngulo de la figura
void TypeF::readTypeF(int a,int b, int c, int an,int bn, int cn){
    setA(a);
    setB(b);
    setC(c);
    setAn(an);
    setBn(bn);
    setCn(cn);
}
