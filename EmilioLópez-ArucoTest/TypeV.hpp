#ifndef _TYPEV_H_
#define _TYPEV_H_
#include <iostream>
#include <cstdio>
using namespace std;

namespace aruco_test{
	class TypeV{
	private:
		float x, y, z;	//coordenadas x, y, z del vértice de un punto de la figura
	public:
		inline TypeV(float x=0.0, float y=0.0, float z=0.0){
			setX(x);
			setY(y);
			setZ(z);
		}

		inline void setX(float v){
			x = v;
		}

		inline void setY(float v){
			y = v;
		}

		inline void setZ(float v){
			z = v;
		}

		inline float getX(){
			return x;
		}

		inline float getY(){
			return y;
		}

		inline float getZ(){
			return z;
		}

		void readTypeV(float x, float y, float z);

	};
}
#endif
