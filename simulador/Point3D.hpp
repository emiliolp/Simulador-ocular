/**
 * Definición de la clase Point3D
 * Determina un punto 3d con las operaciones realizadas para el sistema actual
 */
#ifndef _POINT3D_H_
#define _POINT3D_H_
#include <cmath>

namespace simulator{

    /**\brief Clase que representa un punto 3d
     *
     */
	class Point3D{

        private:

            float x;    /**<@brief distancia x*/
            float y;    /**<@brief distancia y*/
            float z;    /**<@brief distancia z*/

        public:

            /**
             *@brief Constructor de la clase
             *
             *Crea una instancia de la clase que representa un punto 3d
             *@param x distancia x
             *@param y distancia y
             *@param z distancia z
             */
            inline Point3D(float x = 0.0, float y = 0.0, float z = 0.0){

                setX(x);
                setY(y);
                setZ(z);
            }

            /**
             *@brief Almacena la distancia x
             *@param v valor de la distancia
             */
            inline void setX(float v){
                x = v;
            }

            /**
             *@brief Almacena la distancia y
             *@param v valor de la distancia
             */
            inline void setY(float v){
                y = v;
            }

            /**
             *@brief Almacena la distancia z
             *@param v valor de la distancia
             */
            inline void setZ(float v){
                z = v;
            }

            /**
             *@brief Devuelve la distancia x
             */
            inline float getX(){
                return x;
            }

            /**
             *@brief Devuelve la distancia y
             */
            inline float getY(){
                return y;
            }

            /**
             *@brief Devuelve la distancia z
             */
            inline float getZ(){
                return z;
            }

            /**
             *@brief Almacena la posición de un punto
             *@param x distancia x
             *@param y distancia y
             *@param z distancia z
             */
            void readPoint3D(float x, float y, float z);

            /**
             *@brief Debido a que la posición obtenida del tag en un primer momento sitúa el origen en la esquina superor izquierda
             *de la pantalla, este método lo traslada al centro
             *@param x distancia x
             *@param y distancia y
             *@param z distancia z
             */
            void translateCentre();

            /**
             *@brief Ángulo de rotación entorno al eje x
             *
             *Devuelve el valor de la arcotangente de z e y
             */
            float turnAxisX();

            /**
             *@brief Ángulo de rotación entorno al eje y
             *
             *Devuelve el valor de la arcotangente de z y x
             */
            float turnAxisY();
	};
}
#endif
