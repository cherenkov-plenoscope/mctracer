// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <sstream>
#include <exception>
#include "merlict/Rot3.h"

namespace merlict {

Rot3::Rot3() {}

Rot3::Rot3(double rot_x, double rot_y, double rot_z) {
    set(rot_x, rot_y, rot_z);
}

void Rot3::set(double rot_x, double rot_y, double rot_z) {
    _uses_xyz_angels = true;
    Rx = rot_x;
    Ry = rot_y;
    Rz = rot_z;
}

Rot3::Rot3(
    const Vec3 new_rot_axis,
    const double new_rot_angle
) {
    set(new_rot_axis, new_rot_angle);
}

void Rot3::set(
    const Vec3 new_rot_axis,
    const double new_rot_angle
) {
    if (new_rot_angle == 0.0) {
        set(0.0, 0.0, 0.0);
    } else {
        _uses_xyz_angels = false;
        _rot_axis = new_rot_axis/new_rot_axis.norm();
        _rot_angle = new_rot_angle;
    }
}

Vec3 Rot3::rot_axis()const {
    if (_uses_xyz_angels == true) {
        throw std::runtime_error(
            "Rot3::rot_axis():\n"
            "rot_axis was not set!\n");
    } else {
        return _rot_axis;
    }
}

double Rot3::rot_angle()const {
    if (_uses_xyz_angels == true) {
        throw std::runtime_error(
            "Rot3::rot_angle():\n"
            "rot_angle was not set!\n");
    } else {
        return _rot_angle;
    }
}

bool Rot3::uses_xyz_angels()const {
    return _uses_xyz_angels;
}

std::string Rot3::str()const {
    std::stringstream out;
    if (_uses_xyz_angels == true) {
        out << "(" << Rx << " " << Ry << " " << Rz << ")rad";
    } else {
        out << "rot-axis: " << _rot_axis.str() << ", rot-angle: "
            << _rot_angle << "rad";
    }
    return out.str();
}

double Rot3::rot_x()const {return Rx;}
double Rot3::rot_y()const {return Ry;}
double Rot3::rot_z()const {return Rz;}

double Rot3::cosRx() const {return cos(Rx);}
double Rot3::cosRy() const {return cos(Ry);}
double Rot3::cosRz() const {return cos(Rz);}
double Rot3::sinRx() const {return sin(Rx);}
double Rot3::sinRy() const {return sin(Ry);}
double Rot3::sinRz() const {return sin(Rz);}

bool Rot3::operator == (const Rot3& eqRot)const {
    return Rx == eqRot.Rx && Ry == eqRot.Ry && Rz == eqRot.Rz;
}

}  // namespace merlict
