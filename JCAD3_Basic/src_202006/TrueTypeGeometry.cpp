/////////////////////////////////////////////////////////////////////////////
// TrueTypeジオメトリクラス定義
// TrueTypeGeometry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "TrueTypeVector.h"
#include "TrueTypeGeometry.h"

///////////////////////////////////////////////////////////////////////////////
// TRIANGLELIST

/******************************/
void TRIANGLELIST::Empty(void)
/******************************/
{
	int               i;
	TRIANGLELISTLINK* tempnext;

	gotoFirst();
	for(i=0; i<count; i++) {
		delete current->Obj();
		gotoNext();
	}
	gotoFirst();
	for(i=0; i<count; i++) {
		tempnext = current->Next();
		if(current!=NULL) {
			delete current;
		}
		current = tempnext;
	}
	count = 0;
	first = NULL;
	current = NULL;
	last = NULL;
}

/***************************************/
int TRIANGLELIST::Add(TRIANGLE* object)
/***************************************/
{
	TRIANGLELISTLINK* newlink;

	newlink = new TRIANGLELISTLINK(object,NULL);
	if(!newlink) {
		return FALSE;
	}
	if(count==0) {
		first = newlink;
		current = newlink;
		last = newlink;
		last->setNext(NULL);
	} else {
		last->setNext(newlink);
		last = newlink;
	}
	count++;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// POLYGON

/**********************/
POLYGON::POLYGON(void)
/**********************/
{
	numpoints = 0;
	pointlist = NULL;
	orientation = CLOCKWISE;
}

/**********************************/
int POLYGON::findOrientation(void)
/**********************************/
{
	double area;
	int    i;

	area = pointlist[numpoints-1].x * pointlist[0].y
			- pointlist[0].x * pointlist[numpoints-1].y;
	for(i=0; i<numpoints-1; i++) {
		area += pointlist[i].x * pointlist[i+1].y
				- pointlist[i+1].x * pointlist[i].y;
	}
	if(area>=0.0) {
		return ANTICLOCKWISE;
	} else {
		return CLOCKWISE;
	}
}

/****************************************************/
int POLYGON::findDeterminant(int p1, int p2, int p3)
/****************************************************/
{
	double determinant;

	determinant = (pointlist[p2].x-pointlist[p1].x)
                 *(pointlist[p3].y-pointlist[p1].y)
                 -(pointlist[p3].x-pointlist[p1].x)
                 *(pointlist[p2].y-pointlist[p1].y);
	if(determinant>0.0) {
		return ANTICLOCKWISE;
	} else if(determinant==0.0) {
		if(pointlist[p1]==pointlist[p2]||
			pointlist[p1]==pointlist[p3]||
			pointlist[p2]==pointlist[p3]) {
			return CLOCKWISE;
		} else {
			return ANTICLOCKWISE;
		}
	} else {
		return CLOCKWISE;
	}
}

/******************************************************************/
int POLYGON::noneInside(int p1, int p2, int p3, int n, int* vlist)
/******************************************************************/
{
	int i, p;

	for(i=0; i<n; i++) {
		p = vlist[i];
		if(p==p1||p==p2||p==p3) {
			continue;
		}
		if(findDeterminant(p2,p1,p)==orientation||
           findDeterminant(p1,p3,p)==orientation||
           findDeterminant(p3,p2,p)==orientation) {
			continue;
		} else {
			if((pointlist[p].x==pointlist[p1].x&&pointlist[p].y==pointlist[p1].y)||
               (pointlist[p].x==pointlist[p2].x&&pointlist[p].y==pointlist[p2].y)||
               (pointlist[p].x==pointlist[p3].x&&pointlist[p].y==pointlist[p3].y)) {
				continue;
			} else {
				return FALSE;
			}
		}
	}
	return TRUE;
}

/***********************/
void POLYGON::Correct()
/***********************/
{
	int i, j; 

	for(i=0; i<numpoints-1; i++) {
		if(pointlist[i]==pointlist[i+1]) {
			for(j=i; j<numpoints-1; j++) {
				pointlist[j]=pointlist[j+1];
			}
			numpoints--, i--;
		}
	}
}

/***********************************************/
int POLYGON::Triangulate(TRIANGLELIST& trilist)
/***********************************************/
{
	TRIANGLE* current_triangle;
	int       i, previous, current, next, done;
	int       vertex_count, current_determinant, current_position;
	int*      rvl;
	vector    p1, p2, p3, n1(0,0,1), n2(0,0,1), n3(0,0,1);
	char*     progChar  = "/-\\|";	//  Character Array to animate progress with
	int       progCount = 0;			//  Keeps track of progress
	int       progSize  = 4;			//  Number of characters in animation sequence

	rvl = new int[numpoints];
	for(i=0; i<numpoints; i++) {
		rvl[i] = i;
	}
	vertex_count = numpoints;
	while(vertex_count>3) {
		done=FALSE;
		previous=vertex_count-1;
		current=0;
		next=1;
		while(current<vertex_count&&!done) {
			previous = current-1;
			next     = current+1;
			if(current==0) {
				previous=vertex_count-1;
			} else if(current==vertex_count-1) {
				next=0;
			}
			current_determinant =
			findDeterminant(rvl[previous], rvl[current], rvl[next]);
			current_position =
			noneInside(rvl[previous], rvl[current], rvl[next], vertex_count, rvl);
			if((current_determinant==orientation)&&(current_position==TRUE)) {
				done=TRUE;
			} else {
				current++;
			}
		}
		if(!done) {
			return GEOM_ERR_NoPolyFound;
		}
		p1 = vector(pointlist[rvl[previous]]);
		p2 = vector(pointlist[rvl[current]]);
		p3 = vector(pointlist[rvl[next]]);
		current_triangle = new TRIANGLE(p1,p2,p3,n1,n2,n3);
		trilist.Add(current_triangle);
		vertex_count-=1;
		for(i=current; i<vertex_count; i++) {
			rvl[i] = rvl[i+1];
		}
	}
	if(vertex_count==3) {
		p1 = vector(pointlist[rvl[0]]);
		p2 = vector(pointlist[rvl[1]]);
		p3 = vector(pointlist[rvl[2]]);
		current_triangle = new TRIANGLE(p1,p2,p3,n1,n2,n3);
		trilist.Add(current_triangle);
	}
	if(rvl) {
		delete rvl;
	}
	return GEOM_ERR_NoError;
}

/*********************************/
void POLYGON::Combine(POLYGON& p)
/*********************************/
{
	int     i, ni, j;
	double  current_dist, min_dist, distCP, distCN, distMP, distMN;
	int     min_i=0, min_j=0;
	vector  currToPrev, currToNext, mintoPrev, mintoNext, testvector;
	vector* newpl;

	newpl = new vector[numpoints+p.numpoints+2];
	min_dist = BIG;
	for(i=0; i<numpoints; i++) {
		for(j=0; j<p.numpoints; j++) {
			current_dist = dist(pointlist[i],p.pointlist[j]);
			if(current_dist==min_dist) {
				if(i>0) {
					currToPrev = pointlist[i-1]-pointlist[i];
				} else {
					currToPrev = pointlist[numpoints-1]-pointlist[i];
				}
				if(i<numpoints-1) {
					currToNext = pointlist[i+1]-pointlist[i];
				} else {
					currToNext = pointlist[0]-pointlist[i];
				}
				if(min_i>0) {
					mintoPrev = pointlist[min_i-1]-pointlist[min_i];
				} else {
					mintoPrev = pointlist[numpoints-1]-pointlist[min_i];
				}
				if(min_i<numpoints-1) {
					mintoNext = pointlist[min_i+1]-pointlist[min_i];
				} else {
					mintoNext = pointlist[0]-pointlist[min_i];
				}
				testvector = p.pointlist[j]-pointlist[i];
				distCP = dist(~currToPrev,testvector);					// Changed from being normalized...
				distCN = dist(~currToNext,testvector);
				distMP = dist(~mintoPrev ,testvector);
				distMN = dist(~mintoNext ,testvector);
				if((distCP+distCN)<(distMP+distMN)) {
					min_dist = current_dist;
					min_i = i;
					min_j = j;
				}
			} else if(current_dist<min_dist) {
				min_dist = current_dist;
				min_i = i;
				min_j = j;
			}
		}
	}
	ni = 0;
	for(i=0; i<=min_i; i++) {
		newpl[ni]=pointlist[i];
		ni++;
	}
	for(i=min_j; i<p.numpoints; i++) {
		newpl[ni]=p.pointlist[i];
		ni++;
	}
	for(i=0; i<=min_j; i++) {
		newpl[ni]=p.pointlist[i];
		ni++;
	}
	for(i=min_i; i<numpoints; i++) {
		newpl[ni]=pointlist[i];
		ni++;
	}
	numpoints = ni;
	if(pointlist) {
		delete pointlist;
	}
	pointlist = newpl;
}

/*********************************/
int POLYGON::isInside(POLYGON& p)
/*********************************/
{
	int    i, j, c=0;
	double x, y;

	x = pointlist[0].x;
	y = pointlist[0].y;
	for(i=0, j=p.numpoints-1; i<p.numpoints; j=i++) {
		if((((p.pointlist[i].y<=y)&&(y<p.pointlist[j].y))||
			((p.pointlist[j].y<=y)&&(y<p.pointlist[i].y)))&&
			(x<(p.pointlist[j].x-p.pointlist[i].x)*(y-p.pointlist[i].y)/
			(p.pointlist[j].y-p.pointlist[i].y)+p.pointlist[i].x)) {
			c = !c;
		}
	}
	return c;
}

/************************************/
void POLYGON::SetDepth(double depth)
/************************************/
{
	for(int i=0; i<numpoints; i++) {
		pointlist[i].z = depth;
	}
}

/*******************************************************************************/
vector ApproximateQuadraticSpline(vector cp1, vector cp2, vector cp3, double t)
/*******************************************************************************/
{
	double i1 = (1-t)*(1-t);
	double i2 = 2*t*(1-t);
	double i3 = t*t;
	double tx = i1*cp1.x + i2*cp2.x + i3*cp3.x;
	double ty = i1*cp1.y + i2*cp2.y + i3*cp3.y;
	double tz = cp1.z;
	return vector(tx, ty, tz);
}
