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

//=================================
class Sphere :public SurfaceEntity{
protected:
	double radius;
public:
	//======================
	Sphere();
	//======================
	bool set_sphere(double nradius);
	//======================
	void disp();
	//======================
	std::string get_sphere_string();
	//======================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __SPHERE_H_INCLUDED__
