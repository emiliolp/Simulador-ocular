#ifndef _TYPEV_H_
#define _TYPEV_H_
#include <iostream>
#include <cstdio>
using namespace std;

namespace aruco_test{
	class TypeV{
	private:
		float _x, _y, _z;	//coordenadas x, y, z del vértice de un punto de la figura
	public:
		inline TypeV(float _x=0.0, float _y=0.0, float _z=0.0){
			setX(_x);
			setY(_y);
			setZ(_z);
		}

		inline void setX(float v){
			_x = v;
		}

		inline void setY(float v){
			_y = v;
		}

		inline void setZ(float v){
			_z = v;
		}

		inline float getX(){
			return _x;
		}

		inline float getY(){
			return _y;
		}

		inline float getZ(){
			return _z;
		}

		void readTypeV(float x, float y, float z);

	};
}
#endif
