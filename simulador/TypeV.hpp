/**
 * Definici�n de la clase TypeV
 * Determina el valor de una l�nea con cabecera v
 * Tambi�n se utiliza para la representaci�n de una l�nea vn debido a que coincide en el n�mero de par�metros
 * y las operaciones para tratar este tipo son similares
 */
#ifndef _TYPEV_H_
#define _TYPEV_H_
#include <cmath>

namespace simulator{

    /**\brief Definici�n de la clase TypeV
     *
     * Determina las coordenadas de un v�rtice
     */
	class TypeV{

        private:

            float x;    /**<@brief distancia x*/
            float y;    /**<@brief distancia y*/
            float z;	/**<@brief distancia z*/

        public:

            /**
             *@brief Constructor de la clase
             *
             *Crea una instancia de la clase que representa un v�rtice de la figura
             *@param x distancia x
             *@param y distancia y
             *@param z distancia z
             */
            inline TypeV(float x=0.0, float y=0.0, float z=0.0){
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
             *@brief Almacena la distancia u
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
             *@brief Almacena la posici�n de un v�rtice
             *@param x distancia x
             *@param y distancia y
             *@param z distancia z
             */
            void readTypeV(float x, float y, float z);

	};
}
#endif
