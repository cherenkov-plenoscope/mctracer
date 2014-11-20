//=================================
// include guard
#ifndef __CYLINDER_H_INCLUDED__
#define __CYLINDER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "SurfaceEntity.h"
#include "Vector3D.h"
#include "Intersection.h"

using namespace std;

//=================================
class Cylinder :public SurfaceEntity{
protected:
	double CylinderRadius;
	double CylinderLength;
public:
//==================================================================
Cylinder();
//==================================================================
void set_cylinder(	double new_dbl_cylinder_radius,
					Vector3D vec_start, Vector3D vec_end);
//==================================================================
void set_cylinder(	double new_dbl_cylinder_radius,
					double new_dbl_cylinder_length);
//==================================================================
void disp()const;
//==================================================================
std::string get_cylinder_string()const;
//==================================================================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
private:
void set_cylinder_length(const double CylinderLength);
void set_cylinder_length(const Vector3D start_pos, const Vector3D end_pos);
void set_cylinder_radius(const double new_cylinder_radius);
void post_initialize_radius_of_enclosing_sphere();
void assert_start_and_end_point_are_distinct(
	Vector3D start_pos, Vector3D end_pos
);
void set_position_and_orientation(
	const Vector3D start_pos,
	const Vector3D end_pos
);
Rotation3D calculate_new_rotation_in_mother(const Vector3D rotsym_axis);
bool is_paralell_to_z_axis(const Vector3D direction)const;
};
#endif // __CYLINDER_H_INCLUDED__
