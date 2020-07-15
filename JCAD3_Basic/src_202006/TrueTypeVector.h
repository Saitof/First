/////////////////////////////////////////////////////////////////////////////
// TrueTypeフォント VECTORクラス(vector)宣言
// TrueTypeVector.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

class vector
{
	public:  double  x,y,z;

	vector ();
	vector (double _x, double _y, double _z);
	vector (const vector& v);

	vector& operator += (vector v);
	vector& operator -= (vector v);
	vector& operator *= (double f);
	vector& operator /= (double f);
};

// コンストラクション
inline vector::vector(void) {
	x=0; y=0; z=0;
}

inline vector::vector(double _x, double _y, double _z) {
	x=_x; y=_y; z=_z;
}

inline vector::vector(const vector& v) {
	x=v.x; y=v.y; z=v.z;
}

// NON-MEMBER FUNCTIONS
inline vector operator + (vector v1, vector v2) {
	return vector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

inline vector operator - (vector v1, vector v2) {
	return vector(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

inline vector operator * (vector v, double f) {
	return vector(v.x*f, v.y*f, v.z*f);
}

inline vector operator * (double f, vector v) {
	return vector(v.x*f, v.y*f, v.z*f);
}

inline vector operator / (vector v, double f) {
	return vector(v.x/f, v.y/f, v.z/f);
}

inline double operator % (vector v1, vector v2) {
	return (v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
}

inline vector operator ^ (vector v1, vector v2) {
	double  tx = v1.y*v2.z - v1.z*v2.y;
	double  ty = v1.z*v2.x - v1.x*v2.z;
	double  tz = v1.x*v2.y - v1.y*v2.x;
	return  vector(tx, ty, tz);
}

inline vector operator ~ (vector v) {
	double  mag = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	double  tx = v.x/mag;
	double  ty = v.y/mag;
	double  tz = v.z/mag;
	return  vector(tx, ty, tz);
}

inline vector operator - (vector v) {
	return vector(-v.x, -v.y, -v.z);
}

inline int operator == (vector v1, vector v2) {
	return (v1.x==v2.x&&v1.y==v2.y&&v1.z==v2.z);
}

inline int operator != (vector v1, vector v2) {
	return (v1.x!=v2.x&&v1.y!=v2.y&&v1.z!=v2.z);
}

inline double dist (vector v1, vector v2) {
	vector d = v2-v1;
	return (sqrt(d%d));
}

inline vector midpoint (vector v1, vector v2) {
	return ((v1+v2)/2);
}

inline double length (vector v) {
	return (sqrt(v.x*v.x+v.y*v.y+v.z*v.z));
}

// MEMBER SHORTHAND OPERATORS
inline vector& vector::operator += (vector v) {
	x+=v.x; y+=v.y; z+=v.z;
	return *this;
}

inline vector& vector::operator -= (vector v) {
	x-=v.x; y-=v.y; z-=v.z;
	return *this;
}

inline vector& vector::operator *= (double f) {
	x*=f; y*=f; z*=f;
	return *this;
}

inline vector& vector::operator /= (double f) {
	x/=f; y/=f; z/=f;
	return *this;
}

#endif
