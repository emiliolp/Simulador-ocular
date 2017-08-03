/**
 *Implementación de los métodos de la clase TypeF
 */
#include "TypeF.hpp"

using namespace simulator;

//Almacena los índices de los vértices de un triángulo de la figura
void TypeF::readTypeF(int a,int b, int c, int an,int bn, int cn){
    setA(a);
    setB(b);
    setC(c);
    setAn(an);
    setBn(bn);
    setCn(cn);
}
