/**
 * Definici�n de la clase TypeF
 * Determina el valor de una l�nea con cabecera f de un fichero obj
 */
#ifndef _TYPEF_H_
#define _TYPEF_H_
#include <cmath>

namespace simulator{

    /**\brief Definici�n de la clase TypeF
     *
     * Determina los puntos que forma un tri�ngulo que compone una determinada figura
     */
	class TypeF{

        private:

            float a;    /**<@brief �ndice del primer v�rtice*/
            float b;    /**<@brief �ndice del segundo v�rtice*/
            float c;	/**<@brief �ndice del tercer v�rtice*/
            float an;   /**<@brief posici�n de la normal del primer v�rtice*/
            float bn;   /**<@brief posici�n de la normal del segundo v�rtice*/
            float cn;	/**<@brief posici�n de la normal del tercer v�rtice*/

        public:

            /**
             *@brief Constructor de la clase
             *
             *Crea una instancia de la clase que representa la posici�n de los v�rtices de la figura
             *@param a �ndice del primer v�rtice
             *@param b �ndice del segundo v�rtice
             *@param c �ndice del tercer v�rtice
             *@param an �ndice de la normal del primer v�rtice
             *@param bn �ndice de la normal del segundo v�rtice
             *@param cn �ndice de la normal del tercer v�rtice
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
             *@brief Almacena el primer v�rtice del tri�ngulo
             *@param v �ndice del v�rtice
             */
            inline void setA(int v){
                a = v;
            }

            /**
             *@brief Almacena el segundo v�rtice del tri�ngulo
             *@param v �ndice del v�rtice
             */
            inline void setB(int v){
                b = v;
            }

            /**
             *@brief Almacena el tercer v�rtice del tri�ngulo
             *@param v �ndice del v�rtice
             */
            inline void setC(int v){
                c = v;
            }

            /**
             *@brief Almacena la normal del primer v�rtice del tri�ngulo
             *@param v �ndice de la normal
             */
            inline void setAn(int v){
                an = v;
            }

            /**
             *@brief Almacena la normal del segundo v�rtice del tri�ngulo
             *@param v �ndice de la normal
             */
            inline void setBn(int v){
                bn = v;
            }

            /**
             *@brief Almacena la normal del tercer v�rtice del tri�ngulo
             *@param v �ndice de la normal
             */
            inline void setCn(int v){
                cn = v;
            }

            /**
             *@brief Devuelve el primer v�rtice del tri�ngulo
             */
            inline int getA(){
                return a;
            }

            /**
             *@brief Devuelve el segundo v�rtice del tri�ngulo
             */
            inline int getB(){
                return b;
            }

            /**
             *@brief Devuelve el tercer v�rtice del tri�ngulo
             */
            inline int getC(){
                return c;
            }

            /**
             *@brief Devuelve la posici�n de la normal del primer v�rtice
             */
            inline int getAn(){
                return an;
            }

            /**
             *@brief Devuelve la posici�n de la normal del segundo v�rtice
             */
            inline int getBn(){
                return bn;
            }

            /**
             *@brief Devuelve la posici�n de la normal del tercer v�rtice
             */
            inline int getCn(){
                return cn;
            }

            /**
             *@brief Almacena los �ndices de los v�rtices de un tri�ngulo de la figura
             *@param a �ndice del primer v�rtice
             *@param b �ndice del segundo v�rtice
             *@param c �ndice del tercer v�rtice
             *@param an �ndice de la normal del primer v�rtice
             *@param bn �ndice de la normal del segundo v�rtice
             *@param cn �ndice de la normal del tercer v�rtice
             */
            void readTypeF(int a,int b, int c, int an,int bn, int cn);
    };
}
#endif
