// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <sstream>
#include <exception>
#include "merlict/Rot3.h"

namespace merlict {

Rot3::Rot3() {}

Rot3::Rot3(double Phi, double The, double Psi) {
    set(Phi, The, Psi);
}

void Rot3::set(double Phi, double The, double Psi) {
    flag_rot_angles_xyz = true;

    Rx = Phi;
    Ry = The;
    Rz = Psi;
}

Rot3::Rot3(
    const Vec3 new_rot_axis,
    const double new_rot_angle_in_rad
) {
    set(new_rot_axis, new_rot_angle_in_rad);
}

void Rot3::set(
    const Vec3 new_rot_axis,
    const double new_rot_angle_in_rad
) {
    if (new_rot_angle_in_rad == 0.0) {
        set(0.0, 0.0, 0.0);
    } else {
        flag_rot_angles_xyz = false;
        rot_axis = new_rot_axis/new_rot_axis.norm();
        rot_angle_in_rad = new_rot_angle_in_rad;
    }
}

Vec3 Rot3::get_rot_axis()const {
    if (flag_rot_angles_xyz == true)
        throw std::runtime_error(
            "Rot3::get_rot_axis():\n"
            "rot_axis was not set! Returning default ez.\n");
    else
        return rot_axis;
}

double Rot3::get_rot_angle_in_rad()const {
    if (flag_rot_angles_xyz == true)
        throw std::runtime_error(
            "Rot3::get_rot_angle_in_rad():\n"
            "rot_angle_in_rad was not set!");
    else
        return rot_angle_in_rad;
}

bool Rot3::uses_xyz_angels()const {
    return flag_rot_angles_xyz;
}

std::string Rot3::str()const {
    std::stringstream out;

    if (flag_rot_angles_xyz == true)
        out << "(" << Rx << " " << Ry << " " << Rz << ")rad";
    else
        out << "rot-axis: " << rot_axis.str() << ", rot-angle: "
            << rot_angle_in_rad << "rad";

    return out.str();
}

double Rot3::get_rot_x()const {return Rx;}
double Rot3::get_rot_y()const {return Ry;}
double Rot3::get_rot_z()const {return Rz;}

double Rot3::cosRx() const {return cos(Rx);}
double Rot3::cosRy() const {return cos(Ry);}
double Rot3::cosRz() const {return cos(Rz);}
double Rot3::sinRx() const {return sin(Rx);}
double Rot3::sinRy() const {return sin(Ry);}
double Rot3::sinRz() const {return sin(Rz);}

bool Rot3::operator == (const Rot3& eqRot)const {
    return Rx == eqRot.Rx && Ry == eqRot.Ry && Rz == eqRot.Rz;
}

}  // namespace relleums
