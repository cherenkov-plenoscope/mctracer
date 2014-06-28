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
#include "Vector3D.h"
//=================================
// the actual class
class Rotation3D {

	private:
	// modus
	bool flag_rot_angles_xyz;
	// rot angles xyz
	double Rx;
	double Ry;
	double Rz;
	// rot_axis mode
	Vector3D 	rot_axis;
	double 	rot_angle_in_rad;

	public:
	//======================
	Rotation3D();
	//======================
	Rotation3D(double Phi, double The, double Psi);
	void set(double Phi,double The,double Psi);
	void set_null_rotation(){ Rx = 0.0; Ry = 0.0; Rz = 0.0;};
	//======================
	Rotation3D(const Vector3D new_rot_axis,const double new_rot_angle_on_rad);
	void set(const Vector3D new_rot_axis,const double new_rot_angle_on_rad);
	//======================
	Vector3D get_rot_axis() const;
	//======================
	double get_rot_angle_in_rad() const;
	//======================
	bool get_flag_is_rot_angles_xyz() const;
	//======================
	void disp() const;
	//======================
	std::string get_string() const;
	//======================
	double get_rot_x()const;
	double get_rot_y()const;
	double get_rot_z()const;
	//======================
	double cosRx() const;
	double cosRy() const;
	double cosRz() const;
	double sinRx() const;
	double sinRy() const;
	double sinRz() const;
	//======================
	bool operator == (const Rotation3D& eqRot)const;
	friend std::ostream& operator<<(std::ostream& os, const Rotation3D& rot);
};

#endif // __ROTATION3D_H_INCLUDED__ 
