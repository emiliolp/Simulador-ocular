/**
 *Implementación de los métodos de la clase Point3D
 */
#include "Point3D.hpp"

using namespace simulator;

//Guardamos valores x e y
void Point3D::readPoint3D(float x, float y, float z){
    setX(x);
    setY(y);
    setZ(z);
}

//Modificamos coordenadas x,y, trasladando el punto 0,0 desde la esquina superior izquierda al centro de la pantalla
void Point3D::translateCentre(){
    float x, y;
    x = getX() - (640/2);
    y = ((480 / 2) - getY()) + 240;

    setX(x);
    setY(y);
}

//Giramos objeto mediante el eje x
float Point3D::turnAxisX(){
    float angle;
    angle = atan2(getY(),getZ());	    //el ángulo de rotación de x se obtiene a partir de la arcotangente de z e y
    angle = (angle * 180) / 3.141618;   //pasando a radianes
    angle = -angle;

    return angle;
}

//Giramos objeto mediante el eje y
float Point3D::turnAxisY(){
    float angle;
    angle = atan2(getX(),getZ());	    //el ángulo de rotación de y se obtiene a partir de la arcotangente de z y x
    angle = (angle * 180) / 3.141618;   //pasando a radianes
    angle = -angle;

    return angle;
}
