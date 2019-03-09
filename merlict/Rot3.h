// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_ROT3_H_
#define MERLICT_ROT3_H_

#include <string>
#include "merlict/Vec3.h"

namespace merlict {
// Tait–Bryan angles with an extrinsic coordinate frame fasion

class Rot3 {
    double Rx;
    double Ry;
    double Rz;
    double _rot_angle;
    Vec3 _rot_axis;
    bool flag_rot_angles_xyz;

 public:
    Rot3();
    Rot3(double Phi, double The, double Psi);
    Rot3(const Vec3 new_rot_axis, const double new_rot_angle_on_rad);
    void set(double Phi, double The, double Psi);
    void set(const Vec3 new_rot_axis, const double new_rot_angle_on_rad);
    Vec3 rot_axis()const;
    double rot_angle()const;
    bool uses_xyz_angels()const;
    std::string str()const;
    double rot_x()const;
    double rot_y()const;
    double rot_z()const;
    double cosRx()const;
    double cosRy()const;
    double cosRz()const;
    double sinRx()const;
    double sinRy()const;
    double sinRz()const;
    bool operator == (const Rot3& eqRot)const;
};

const Rot3 ROT3_UNITY = Rot3(0.0, 0.0, 0.0);

}  // namespace merlict

#endif  // MERLICT_ROT3_H_
