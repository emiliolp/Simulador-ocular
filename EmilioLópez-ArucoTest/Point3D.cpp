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
    //con 502px de perímetro 20 cm de profundidad, el perímetro del marcador es inversamente proporcional a su profundidad (Cámara normal)
    //setZ((500 * 20) / perimeter);
    //con 272px de perímetro 20 cm de profundidad, (Cámara pez)
    setZ((272 * 20) / perimeter);
}

//Convertimos cm en px: 100 px equivale a 2.646 cm (anteriofr 2.5)
void Point3D::convertToPixel(){
    setZ((getZ() * 100) / 2.646);
}

//Modificamos coordenadas x,y, trasladando el punto 0,0 desde la esquina superior izquierda al centro de la pantalla
void Point3D::translateCentre(int position){
    //float x = getX() - (640/2);		//x' = x - 320 (mitad del ancho de la pantalla 320 px)
    //float y = ((480 / 2) - getY())+160;	//y' = 240 - y (la mitad de la altura de la pantalla es 240 px)
    float x, y;

    switch (position)
    {
    case 1:
        x = getX() - (640/2);
        y = ((480 / 2) - getY()) + 160;
        break;

    case 2:
        x = (getX() - (640/2)) + 320;
        y = ((480 / 2) - getY()) + 160;
        break;

    case 3:
        x = (getX() - (640/2)) + 320;
        y = ((480 / 2) - getY()) - 80;
        break;

    case 4:
        x = (getX() - (640/2)) + 320;
        y = ((480 / 2 ) - getY()) - 320;
        break;

    case 5:
        x = getX() - (640/2);
        y = ((480 / 2 ) - getY()) - 320;
        break;

    case 6:
        x = (getX() - (640/2)) - 320;
        y = ((480 / 2 ) - getY()) - 320;
        break;

    case 7:
        x = (getX() - (640/2)) - 320;
        y = ((480 / 2) - getY()) - 80;
        break;

    case 8:
        x = (getX() - (640/2)) - 320;
        y = ((480 / 2) - getY()) + 160;
        break;

    }

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
