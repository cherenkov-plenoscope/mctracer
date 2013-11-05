//=================================
// include guard
#ifndef __TRIANGLE_H_INCLUDED__
#define __TRIANGLE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "SurfaceEntity.h"
#include "Intersection.h"

//=================================
class Triangle  :public SurfaceEntity{
protected:	
	// three corners of a triangle
	Vector3D A;
	Vector3D B;	
	Vector3D C;
public:
//======================
Triangle();
//======================
bool set_Triangle(const	Vector3D A,const Vector3D B,const Vector3D C);
//======================
void disp()const;
//======================
std::string get_Triangle_string()const;
//======================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};

#endif // __TRIANGLE_H_INCLUDED__ 
