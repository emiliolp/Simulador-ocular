/**
 * Definición de la clase TypeF
 * Determina el valor de una línea con cabecera f de un fichero obj
 */
#ifndef _TYPEF_H_
#define _TYPEF_H_
#include <cmath>

namespace simulator{

    /**\brief Definición de la clase TypeF
     *
     * Determina los puntos que forma un triángulo que compone una determinada figura
     */
	class TypeF{

        private:

            float a;    /**<@brief índice del primer vértice*/
            float b;    /**<@brief índice del segundo vértice*/
            float c;	/**<@brief índice del tercer vértice*/
            float an;   /**<@brief posición de la normal del primer vértice*/
            float bn;   /**<@brief posición de la normal del segundo vértice*/
            float cn;	/**<@brief posición de la normal del tercer vértice*/

        public:

            /**
             *@brief Constructor de la clase
             *
             *Crea una instancia de la clase que representa la posición de los vértices de la figura
             *@param a índice del primer vértice
             *@param b índice del segundo vértice
             *@param c índice del tercer vértice
             *@param an índice de la normal del primer vértice
             *@param bn índice de la normal del segundo vértice
             *@param cn índice de la normal del tercer vértice
             */
            inline TypeF(int a=0, int b=0, int c=0, int an=0, int bn=0, int cn=0){
                setA(a);
                setB(b);
                setC(c);
                setAn(an);
                setBn(bn);
                setCn(cn);
            }

            /**
             *@brief Almacena el primer vértice del triángulo
             *@param v índice del vértice
             */
            inline void setA(int v){
                a = v;
            }

            /**
             *@brief Almacena el segundo vértice del triángulo
             *@param v índice del vértice
             */
            inline void setB(int v){
                b = v;
            }

            /**
             *@brief Almacena el tercer vértice del triángulo
             *@param v índice del vértice
             */
            inline void setC(int v){
                c = v;
            }

            /**
             *@brief Almacena la normal del primer vértice del triángulo
             *@param v índice de la normal
             */
            inline void setAn(int v){
                an = v;
            }

            /**
             *@brief Almacena la normal del segundo vértice del triángulo
             *@param v índice de la normal
             */
            inline void setBn(int v){
                bn = v;
            }

            /**
             *@brief Almacena la normal del tercer vértice del triángulo
             *@param v índice de la normal
             */
            inline void setCn(int v){
                cn = v;
            }

            /**
             *@brief Devuelve el primer vértice del triángulo
             */
            inline int getA(){
                return a;
            }

            /**
             *@brief Devuelve el segundo vértice del triángulo
             */
            inline int getB(){
                return b;
            }

            /**
             *@brief Devuelve el tercer vértice del triángulo
             */
            inline int getC(){
                return c;
            }

            /**
             *@brief Devuelve la posición de la normal del primer vértice
             */
            inline int getAn(){
                return an;
            }

            /**
             *@brief Devuelve la posición de la normal del segundo vértice
             */
            inline int getBn(){
                return bn;
            }

            /**
             *@brief Devuelve la posición de la normal del tercer vértice
             */
            inline int getCn(){
                return cn;
            }

            /**
             *@brief Almacena los índices de los vértices de un triángulo de la figura
             *@param a índice del primer vértice
             *@param b índice del segundo vértice
             *@param c índice del tercer vértice
             *@param an índice de la normal del primer vértice
             *@param bn índice de la normal del segundo vértice
             *@param cn índice de la normal del tercer vértice
             */
            void readTypeF(int a,int b, int c, int an,int bn, int cn);
    };
}
#endif
