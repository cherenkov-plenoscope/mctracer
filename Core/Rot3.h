// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_ROT3_H_
#define MCTRACER_CORE_ROT3_H_

#include <string>
#include "Core/Vec3.h"

// Tait–Bryan angles with an extrinsic coordinate frame fasion

class Rot3 {
    double Rx;
    double Ry;
    double Rz;
    double rot_angle_in_rad;
    Vec3 rot_axis;
    bool flag_rot_angles_xyz;

 public:
    static const Rot3 UNITY;
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
#endif  // MCTRACER_CORE_ROT3_H_
