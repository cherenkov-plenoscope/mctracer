//=================================
// include guard
#ifndef __DISC_H_INCLUDED__
#define __DISC_H_INCLUDED__

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
class Disc :public SurfaceEntity{
protected:
	double Radius; 	
public:
	void set_Disc(const double Radius);

	void disp();

	std::string get_Disc_string();

	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
private:
	void set_Disc_radius(const double radius);
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __DISC_H_INCLUDED__
