// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <sstream>
#include <iomanip>
#include "merlict/HomTra3.h"


namespace merlict {

// homogenous Transformation, component adresses:
// [ 0,0    0,1     0,2     0,3 ]
// [ 1,0    1,1     1,2     1,3 ]
// [ 2,0    2,1     2,2     2,3 ]
// [ 3,0    3,1     3,2     3,3 ]
//
// -Rotatin component: Matrix r[3x3]
// -Translation component: Vector t[1x3]
//
// homoT =  [ r(0,0) r(0,1) r(0,2) t(1) ]
//          [ r(1,0) r(1,1) r(1,2) t(2) ]
//          [ r(2,0) r(2,1) r(2,2) t(3) ]
//          [ 0      0      0      1    ]
//

HomTra3::HomTra3():
    T{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}}
{
    // default init is unit matrix
    // [1 0 0 0]
    // [0 1 0 0]
    // [0 0 1 0]
    // [0 0 0 1] //last row is always the same
}

HomTra3::HomTra3(
    const double r00, const double r10, const double r20, const double t30,
    const double r01, const double r11, const double r21, const double t31,
    const double r02, const double r12, const double r22, const double t32
):
    T{{r00, r10, r20, t30}, {r01, r11, r21, t31}, {r02, r12, r22, t32}}
{}

void HomTra3::set_transformation(const Rot3 R, const Vec3 pos) {
    HomTra3 TrafRotation;
    TrafRotation.set_rotation_component(R);

    HomTra3 TrafTranslation;
    TrafTranslation.set_translation_component(pos);

    // composition
    *this = TrafTranslation*TrafRotation;
}

void HomTra3::set_rotation_component(const Rot3 R) {
    if (R.uses_xyz_angels())
        set_rotation_component_based_on_xyz_angles(R);
    else
        set_rotation_component_based_on_rot_axis(R);
}

void HomTra3::set_translation_component(const Vec3 &t) {
    T[0][3] = t.x;
    T[1][3] = t.y;
    T[2][3] = t.z;
}

void HomTra3::set_rotation_component_based_on_rot_axis(const Rot3 R) {
    // ensure rot_axis is a unit vector
    Vec3 rot_axis = R.get_rot_axis();
    rot_axis = rot_axis/rot_axis.norm();

    const double rx = rot_axis.x;
    const double ry = rot_axis.y;
    const double rz = rot_axis.z;

    const double sinR = sin(R.get_rot_angle_in_rad());
    const double cosR = cos(R.get_rot_angle_in_rad());

    // first row
    T[0][0] = cosR +  rx*rx*(1.0-cosR);
    T[0][1] = rx*ry*(1.0-cosR)-rz*sinR;
    T[0][2] = rx*rz*(1.0-cosR)+ry*sinR;
    // second row
    T[1][0] = ry*rx*(1.0-cosR)+rz*sinR;
    T[1][1] = cosR +  ry*ry*(1.0-cosR);
    T[1][2] = ry*rz*(1.0-cosR)-rx*sinR;
    // third row
    T[2][0] = rz*rx*(1.0-cosR)-ry*sinR;
    T[2][1] = rz*ry*(1.0-cosR)+rx*sinR;
    T[2][2] = cosR +  rz*rz*(1.0-cosR);
}

void HomTra3::set_rotation_component_based_on_xyz_angles(const Rot3 R) {
    const double cosRx = R.cosRx();
    const double cosRy = R.cosRy();
    const double cosRz = R.cosRz();
    const double sinRx = R.sinRx();
    const double sinRy = R.sinRy();
    const double sinRz = R.sinRz();
    // first row
    T[0][0] = cosRy*cosRz;
    T[0][1] = cosRx*sinRz + sinRx*sinRy*cosRz;
    T[0][2] = sinRx*sinRz - cosRx*sinRy*cosRz;
    // second row
    T[1][0] =-cosRy*sinRz;
    T[1][1] = cosRx*cosRz - sinRx*sinRy*sinRz;
    T[1][2] = sinRx*cosRz + cosRx*sinRy*sinRz;
    // third row
    T[2][0] = sinRy;
    T[2][1] =-sinRx*cosRy;
    T[2][2] = cosRx*cosRy;
}

void HomTra3::set_transformation(
    Vec3 rotx,
    Vec3 roty,
    Vec3 rotz,
    const Vec3 pos
) {
    rotx = rotx/rotx.norm();
    roty = roty/roty.norm();
    rotz = rotz/rotz.norm();

    HomTra3 TrafRotation;
    TrafRotation.set_x_column_of_rotation_component(rotx);
    TrafRotation.set_y_column_of_rotation_component(roty);
    TrafRotation.set_z_column_of_rotation_component(rotz);

    HomTra3 TrafTranslation;
    TrafTranslation.set_translation_component(pos);

    // composition
    *this = TrafTranslation*TrafRotation;
}

void HomTra3::set_x_column_of_rotation_component(const Vec3 &R) {
    T[0][0] = R.x;
    T[1][0] = R.y;
    T[2][0] = R.z;
}

void HomTra3::set_y_column_of_rotation_component(const Vec3 &R) {
    T[0][1] = R.x;
    T[1][1] = R.y;
    T[2][1] = R.z;
}

void HomTra3::set_z_column_of_rotation_component(const Vec3 &R) {
    T[0][2] = R.x;
    T[1][2] = R.y;
    T[2][2] = R.z;
}

Vec3 HomTra3::get_transformed_orientation(const Vec3& ori)const {
    return Vec3(
        // x
        ori.x*T[0][0] +
        ori.y*T[0][1] +
        ori.z*T[0][2],
        // y
        ori.x*T[1][0] +
        ori.y*T[1][1] +
        ori.z*T[1][2],
        // z
        ori.x*T[2][0] +
        ori.y*T[2][1] +
        ori.z*T[2][2]);
}

Vec3 HomTra3::get_transformed_orientation_inverse(const Vec3& ori)const {
    return Vec3(
        // x
        ori.x*T[0][0] +
        ori.y*T[1][0] +
        ori.z*T[2][0],
        // y
        ori.x*T[0][1] +
        ori.y*T[1][1] +
        ori.z*T[2][1],
        // z
        ori.x*T[0][2] +
        ori.y*T[1][2] +
        ori.z*T[2][2]);
}

Vec3 HomTra3::get_transformed_position(const Vec3& pos)const {
    return Vec3(
        // x
        pos.x*T[0][0] +
        pos.y*T[0][1] +
        pos.z*T[0][2] + T[0][3],
        // y
        pos.x*T[1][0] +
        pos.y*T[1][1] +
        pos.z*T[1][2] + T[1][3],
        // z
        pos.x*T[2][0] +
        pos.y*T[2][1] +
        pos.z*T[2][2] + T[2][3]);
}

Vec3 HomTra3::get_transformed_position_inverse(const Vec3& pos)const {
  return Vec3(
    // x
    pos.x*T[0][0] +
    pos.y*T[1][0] +
    pos.z*T[2][0] - (T[0][0]*T[0][3] + T[1][0]*T[1][3] + T[2][0]*T[2][3]),
    // y
    pos.x*T[0][1] +
    pos.y*T[1][1] +
    pos.z*T[2][1] - (T[0][1]*T[0][3] + T[1][1]*T[1][3] + T[2][1]*T[2][3]),
    // z
    pos.x*T[0][2] +
    pos.y*T[1][2] +
    pos.z*T[2][2] - (T[0][2]*T[0][3] + T[1][2]*T[1][3] + T[2][2]*T[2][3]));
}

Vec3 HomTra3::get_translation()const {
    return Vec3(T[0][3], T[1][3], T[2][3]);
}

Vec3 HomTra3::get_rot_x()const { return Vec3(T[0][0], T[1][0], T[2][0]); }
Vec3 HomTra3::get_rot_y()const { return Vec3(T[0][1], T[1][1], T[2][1]); }
Vec3 HomTra3::get_rot_z()const { return Vec3(T[0][2], T[1][2], T[2][2]); }

std::string HomTra3::str()const {
    std::stringstream out;
    out << std::setprecision(3);
    out << get_single_row_print(0);
    out << get_single_row_print(1);
    out << get_single_row_print(2);
    out << "[  0\t0\t0\t1 ]\n";
    return  out.str();
}

std::string HomTra3::get_single_row_print(const unsigned int r)const {
    std::stringstream out;
    out << std::setprecision(3) << "[  ";
    out << T[r][0] << " \t" << T[r][1] << " \t" << T[r][2] << " \t" << T[r][3];
    out <<"  ]\n";
    return out.str();
}

void HomTra3::operator= (const HomTra3 G) {
    for (unsigned int row = 0; row < 3; row++)
        for (unsigned int col = 0; col < 4; col++)
            T[row][col] = G.T[row][col];
}

HomTra3 HomTra3::operator* (const HomTra3 G)const {
  // Matrix multiplication

  return HomTra3(
    T[0][0]*G.T[0][0] + T[0][1]*G.T[1][0] + T[0][2]*G.T[2][0],  // + T[0][3]*      0.0
    T[0][0]*G.T[0][1] + T[0][1]*G.T[1][1] + T[0][2]*G.T[2][1],  // + T[0][3]*      0.0
    T[0][0]*G.T[0][2] + T[0][1]*G.T[1][2] + T[0][2]*G.T[2][2],  // + T[0][3]*      0.0
    T[0][0]*G.T[0][3] + T[0][1]*G.T[1][3] + T[0][2]*G.T[2][3] + T[0][3],  //*      1.0

    T[1][0]*G.T[0][0] + T[1][1]*G.T[1][0] + T[1][2]*G.T[2][0],  // + T[1][3]*      0.0
    T[1][0]*G.T[0][1] + T[1][1]*G.T[1][1] + T[1][2]*G.T[2][1],  // + T[1][3]*      0.0
    T[1][0]*G.T[0][2] + T[1][1]*G.T[1][2] + T[1][2]*G.T[2][2],  // + T[1][3]*      0.0
    T[1][0]*G.T[0][3] + T[1][1]*G.T[1][3] + T[1][2]*G.T[2][3] + T[1][3],  //*      1.0

    T[2][0]*G.T[0][0] + T[2][1]*G.T[1][0] + T[2][2]*G.T[2][0],  // + T[2][3]*      0.0
    T[2][0]*G.T[0][1] + T[2][1]*G.T[1][1] + T[2][2]*G.T[2][1],  // + T[2][3]*      0.0
    T[2][0]*G.T[0][2] + T[2][1]*G.T[1][2] + T[2][2]*G.T[2][2],  // + T[2][3]*      0.0
    T[2][0]*G.T[0][3] + T[2][1]*G.T[1][3] + T[2][2]*G.T[2][3] + T[2][3]   //*      1.0

 // T[3][0]*G.T[0][0] + T[3][1]*G.T[1][0] + T[3][2]*G.T[2][0] + T[3][3]*G.T[3][0];
 // T[3][0]*G.T[0][1] + T[3][1]*G.T[1][1] + T[3][2]*G.T[2][1] + T[3][3]*G.T[3][1];
 // T[3][0]*G.T[0][2] + T[3][1]*G.T[1][2] + T[3][2]*G.T[2][2] + T[3][3]*G.T[3][2];
 // T[3][0]*G.T[0][3] + T[3][1]*G.T[1][3] + T[3][2]*G.T[2][3] + T[3][3]*G.T[3][3];
  );
}

HomTra3 HomTra3::inverse()const {
    // The inverse Homogenous Transformation is a composition of two components:
    //
    // 1)
    //   inverse_HomTra3_rot =           | 0   The inverse rotation matrix R^-1
    //                             R^-1  | 0   is the same as R^T since rotation
    //                                   | 0   matrices are orthogonal.
    //                          ---------+---  The translation component is (0,0,0).
    //                           0  0  0 | 1
    //
    // 2)
    //   inverse_HomTra3_trans = 1  0  0 | -tx  The rotation part is unity.
    //                           0  1  0 | -ty  The translation part is the
    //                           0  0  1 | -tz  negative translation component
    //                          ---------+---   of the original
    //                           0  0  0 | 1
    //
    // Now these two components are multiplied to compose the inverse HomTra3:
    //
    // inverse_HomTra3 = inverse_HomTra3_rot * inverse_HomTra3_trans
    //
    // The composition done by hand:
    return HomTra3(
        T[0][0], T[1][0], T[2][0], -(T[0][0]*T[0][3] + T[1][0]*T[1][3] + T[2][0]*T[2][3]),
        T[0][1], T[1][1], T[2][1], -(T[0][1]*T[0][3] + T[1][1]*T[1][3] + T[2][1]*T[2][3]),
        T[0][2], T[1][2], T[2][2], -(T[0][2]*T[0][3] + T[1][2]*T[1][3] + T[2][2]*T[2][3]));
}

bool HomTra3::operator== (HomTra3 G)const {
    for (unsigned int row = 0; row < 4; row++)
        for (unsigned int col = 0; col < 3; col++)
            if (T[col][row] != G.T[col][row])
                return false;

    return true;
}

}  // namespace merlict
