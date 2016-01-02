#include <iostream>
#include "Point3D.hpp"
#include <cmath>

using namespace aruco_test;

//Guardamos valores x e y
void Point3D::readPoint3D(float x, float y){
    setX(x);
    setY(y);
}

//Obtenemos el valor z del centro del marcador
void Point3D::obtenerz(float perimeter){
    //con 502 de perímetro 20 cm de profundidad, el perímetro del marcador es inversamente proporcional a su profundidad
    setZ((500 * 20) / perimeter);
}

//Convertimos cm en px
void Point3D::convertToPixel(){
    setZ((getZ() * 100) / 2.5);
}

//Modificamos coordenadas x,y, trasladando el punto 0,0 desde la esquina superior izquierda al centro de la pantalla
void Point3D::translateCentre(){
    float x = getX() - (640/2);		//x' = x - 320 (mitad del ancho de la pantalla 320 px)
    float y = (480 / 2) - getY();	//y' = 240 - y (la mitad de la altura de la pantalla es 240 px)
    setX(x);
    setY(y);
}

//Giramos objeto mediante el eje z
float Point3D::turnAxisZ(){
    float angle;
    angle = atan2(getY(),getX());	//el ángulo de rotación de z se obtiene a partir de la arcotangente de x e y
    angle = (angle * 180) / 3.141618; //pasando a radianes

    return angle;
}

//Giramos objeto mediante el eje x
float Point3D::turnAxisX(){
    float angle;
    angle = atan2(getY(),getZ());	//el ángulo de rotación de x se obtiene a partir de la arcotangente de z e y
    angle = (angle * 180) / 3.141618; //pasando a radianes
    angle = -angle;

    return angle;
}

//Giramos objeto mediante el eje y
float Point3D::turnAxisY(){
    float angle;
    angle = atan2(getX(),getZ());	//el ángulo de rotación de y se obtiene a partir de la arcotangente de z y x
    angle = (angle * 180) / 3.141618; //pasando a radianes
    angle = -angle;

    return angle;
}