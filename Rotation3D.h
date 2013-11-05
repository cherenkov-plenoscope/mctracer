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
bool flag_euler_angles;
// rot angles xyz
double Rx;
double Ry;
double Rz;
// rot_axis mode
Vector3D vec_rot_axis;
double 	dbl_rot_angle_in_rad;
public:
//======================
Rotation3D();
//======================
Rotation3D(double Phi, double The, double Psi);
//======================
void set_rotation(double Phi,double The,double Psi);
//======================
void set_rotation
(const Vector3D vec_new_rot_axis,const double dbl_new_rot_angle_on_rad);
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
double cosPhi() const;
double cosThe() const;
double cosPsi() const;
double sinPhi() const;
double sinThe() const;
double sinPsi() const;
//======================
friend std::ostream& operator<<(std::ostream& os, const Rotation3D& rot);
};
#endif // __ROTATION3D_H_INCLUDED__ 
