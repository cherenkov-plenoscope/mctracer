//=================================
// include guard
#ifndef __Rot3_H_INCLUDED__
#define __Rot3_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vec3.h"
//=================================
// Tait–Bryan angles with an extrinsic coordinate frame fasion
class Rot3 {
	double Rx;
	double Ry;
	double Rz;
	double rot_angle_in_rad;
	Vec3 rot_axis;
	bool flag_rot_angles_xyz;
public:
	static const Rot3 null;
	
	Rot3();
	Rot3(double Phi, double The, double Psi);
	Rot3(const Vec3 new_rot_axis, const double new_rot_angle_on_rad);
	
	void set(double Phi, double The, double Psi);
	void set(const Vec3 new_rot_axis, const double new_rot_angle_on_rad);
	
	Vec3 get_rot_axis()const;
	double get_rot_angle_in_rad()const;
	bool uses_xyz_angels()const;
	std::string str()const;
	double get_rot_x()const;
	double get_rot_y()const;
	double get_rot_z()const;
	double cosRx()const;
	double cosRy()const;
	double cosRz()const;
	double sinRx()const;
	double sinRy()const;
	double sinRz()const;
	bool operator == (const Rot3& eqRot)const;
};
#endif // __Rot3_H_INCLUDED__ 
