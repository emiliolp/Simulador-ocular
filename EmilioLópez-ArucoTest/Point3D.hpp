#ifndef _POINT3D_H_
#define _POINT3D_H_
#include <iostream>

using namespace std;

namespace aruco_test{
	class Point3D{
	private:
		float _x, _y, _z;	//coordenadas x, y, z del marcador

	public:
		inline Point3D(float _x = 0.0, float _y = 0.0, float _z = 0.0){
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

		void readPoint3D(float x, float y);
		void obtenerz(float perimeter);
		void convertToPixel();
		void translateCentre(int position);
		float turnAxisZ();
		float turnAxisX();
		float turnAxisY();
	};
}
#endif
