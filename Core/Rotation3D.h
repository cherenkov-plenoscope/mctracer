//=================================
// include guard
#ifndef __ROTATION3D_H_INCLUDED__
#define __ROTATION3D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>
#include "Core/Vec3.h"
#include "Core/TracerException.h"
#include "Core/Printable.h"
//=================================

class Rotation3D : public Printable{

	bool flag_rot_angles_xyz;

	double Rx;
	double Ry;
	double Rz;

	Vec3 rot_axis;
	double rot_angle_in_rad;
public:
	static const Rotation3D null;
	
	Rotation3D();
	Rotation3D(double Phi, double The, double Psi);
	Rotation3D(const Vec3 new_rot_axis, const double new_rot_angle_on_rad);
	
	void set(double Phi, double The, double Psi);
	void set(const Vec3 new_rot_axis, const double new_rot_angle_on_rad);
	
	Vec3 get_rot_axis()const;
	double get_rot_angle_in_rad()const;
	bool uses_xyz_angels()const;
	std::string get_print()const;
	double get_rot_x()const;
	double get_rot_y()const;
	double get_rot_z()const;
	double cosRx()const;
	double cosRy()const;
	double cosRz()const;
	double sinRx()const;
	double sinRy()const;
	double sinRz()const;
	bool operator == (const Rotation3D& eqRot)const;
};
#endif // __ROTATION3D_H_INCLUDED__ 
