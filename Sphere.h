//=================================
// include guard
#ifndef __SPHERE_H_INCLUDED__
#define __SPHERE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "SurfaceEntity.h"
#include "Intersection.h"
#include "QuadraticEquation.h"

//=================================
class Sphere :public SurfaceEntity{
protected:
	double radius;
public:
	//======================
	Sphere();
	//======================
	void set_sphere(double nradius);
	//======================
	void disp();
	//======================
	std::string get_sphere_string()const;
	//======================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __SPHERE_H_INCLUDED__
