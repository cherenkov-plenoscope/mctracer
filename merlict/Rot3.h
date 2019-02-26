// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_ROT3_H_
#define CORE_ROT3_H_

#include <string>
#include "merlict/Vec3.h"

namespace merlict {
// Tait–Bryan angles with an extrinsic coordinate frame fasion

class Rot3 {
    double Rx;
    double Ry;
    double Rz;
    double rot_angle_in_rad;
    Vec3 rot_axis;
    bool flag_rot_angles_xyz;

 public:
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

const Rot3 ROT3_UNITY = Rot3(0.0, 0.0, 0.0);

}  // namespace relleums

#endif  // CORE_ROT3_H_
