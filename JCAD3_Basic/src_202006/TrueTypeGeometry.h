/////////////////////////////////////////////////////////////////////////////
// TrueTypeジオメトリクラス宣言
// TrueTypeGeometry.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "TrueTypeVector.h"

#define TRUE					1
#define FALSE					0
#define CLOCKWISE				1
#define ANTICLOCKWISE			2
#define BIG						1.0e30
#define MIN_SHRINK_ANGLE		0.15
#define GEOM_ERR_NoError		0
#define GEOM_ERR_NoPolyFound	1

///////////////////////////////////////////////////////////////////////////////
// TRIANGLE

class TRIANGLE
{
public:
	vector v1, v2, v3;
	vector n1, n2, n3;

	TRIANGLE(void) {}
	TRIANGLE(vector _v1, vector _v2, vector _v3) {
		v1=_v1; v2=_v2; v3=_v3;
		n1=vector(0,0,1);
		n2=vector(0,0,1);
		n3=vector(0,0,1);
	}

	TRIANGLE(vector _v1, vector _v2, vector _v3,
				vector _n1, vector _n2, vector _n3) {
		v1=_v1; v2=_v2; v3=_v3;
		n1=_n1; n2=_n2; n3=_n3;
	}

	TRIANGLE(const TRIANGLE& t) {
		v1=t.v1; v2=t.v2; v3=t.v3;
		n1=t.n1; n2=t.n2; n3=t.n3;
	}

	vector Normal(void) {
		return ~((v3-v1)^(v2-v1));
	};

	int Orientation(void) {
		if(((v2.x-v1.x)*(v3.y-v1.y)-(v3.x-v1.x)*(v2.y-v1.y))>=0.0)
			return ANTICLOCKWISE;
		else
			return CLOCKWISE;
	}
};

///////////////////////////////////////////////////////////////////////////////
// TRIANGLELISTLINK

class TRIANGLELISTLINK
{
private:
	TRIANGLE*			obj;
	TRIANGLELISTLINK*	next;

public:
	TRIANGLELISTLINK (void) {
		obj = NULL;
		next = NULL;
	}

	TRIANGLELISTLINK (TRIANGLE* object, TRIANGLELISTLINK* tonext) {
		obj = object;
		next = tonext;
	}

	TRIANGLE* Obj(void) {
		return obj;
	};

	TRIANGLELISTLINK* Next(void) {
		return next;
	};

	void setObj(TRIANGLE* object) {
		obj = object;
	};

	void setNext(TRIANGLELISTLINK* n) {
		next = n;
	};
};

///////////////////////////////////////////////////////////////////////////////
// TRIANGLELIST

class TRIANGLELIST
{
private:
	int					count;
	TRIANGLELISTLINK*	first;
	TRIANGLELISTLINK*	current;
	TRIANGLELISTLINK*	last;

public:
	TRIANGLELIST(void) {
		count = 0;
		first = NULL;
		current = NULL;
		last = NULL;
	}

	void Empty(void);
	int Add(TRIANGLE* t);

	int Count(void) {
		return count;
	};

	TRIANGLE* First(void) {
		return first->Obj();
	};

	TRIANGLE* Current(void) {
		return current->Obj();
	};

	TRIANGLE* Last(void) {
		return last->Obj();
	};

	TRIANGLE* Next(void) {
		return current->Next()->Obj();
	};

	void gotoFirst(void) {
		current = first;
	};

	void gotoLast(void) {
		current = last;
	};

	void gotoNext(void) {
		if(current!=last) current = current->Next();
	};
};

///////////////////////////////////////////////////////////////////////////////
// POLYGON

class POLYGON
{
public:
	int		numpoints;
	vector*	pointlist;
	int		orientation;

	int findOrientation(void);
	int findDeterminant(int p1, int p2, int p3);
	int noneInside(int p1, int p2, int p3, int n, int* vlist);
	POLYGON(void);
	int NumPoints(void) { return numpoints; }
	void Correct(void);
	int Triangulate(TRIANGLELIST& trianglelist);
	int isInside(POLYGON& p);
	void Combine(POLYGON& p);
	void SetDepth(double d);
};

vector ApproximateQuadraticSpline(vector cp1, vector cp2, vector cp3, double t);

#endif
