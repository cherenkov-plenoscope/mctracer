//=================================
// include guard
#ifndef __DISK_H_INCLUDED__
#define __DISK_H_INCLUDED__

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
class Disk :public SurfaceEntity{
protected:
	double dbl_disk_radius_in_m; 	
public:
//==================================================================
Disk();
//==================================================================
bool set_disk(double dbl_new_disk_radius_in_m);
//==================================================================
void disp();
//==================================================================
std::string get_disk_string();
//==================================================================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __DISK_H_INCLUDED__
