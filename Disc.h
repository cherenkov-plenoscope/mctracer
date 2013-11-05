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
#include <vector>
#include "SurfaceEntity.h"
#include "Intersection.h"
//=================================
class Disc :public SurfaceEntity{
protected:
	double dbl_Disc_radius_in_m; 	
public:
//==================================================================
Disc();
//==================================================================
bool set_Disc(double dbl_new_Disc_radius_in_m);
//==================================================================
void disp();
//==================================================================
std::string get_Disc_string();
//==================================================================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __DISC_H_INCLUDED__
