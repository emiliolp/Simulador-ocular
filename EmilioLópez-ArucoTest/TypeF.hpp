#ifndef _TYPEF_H_
#define _TYPEF_H_

namespace aruco_test{
	class TypeF{
	private:
		float _a, _b, _c;		//almacenan la posición de cada vértice del triángulo
		float _an, _bn, _cn;	//almacenan la posición de la normal de cada vértice
	public:
		inline TypeF(int a=0, int b=0, int c=0, int an=0, int bn=0, int cn=0){
			setA(a);
			setB(b);
			setC(c);
			setAn(an);
			setBn(bn);
			setCn(cn);
		}

		inline void setA(int v){
			_a = v;
		}

		inline void setB(int v){
			_b = v;
		}

		inline void setC(int v){
			_c = v;
		}

		inline void setAn(int v){
			_an = v;
		}

		inline void setBn(int v){
			_bn = v;
		}

		inline void setCn(int v){
			_cn = v;
		}

		inline int getA(){
			return _a;
		}

		inline int getB(){
			return _b;
		}

		inline int getC(){
			return _c;
		}

		inline int getAn(){
			return _an;
		}

		inline int getBn(){
			return _bn;
		}

		inline int getCn(){
			return _cn;
		}

		void readTypeF(int a,int b, int c, int an,int bn, int cn);
	};
}
#endif