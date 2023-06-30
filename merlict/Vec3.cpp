// Copyright 2014 Sebastian A. Mueller
#include "merlict/Vec3.h"
#include <math.h>
#include <sstream>

namespace merlict {

Vec3::Vec3() {}

Vec3::Vec3(const double _x, const double _y, const double _z):
    x(_x), y(_y), z(_z)
{}

void Vec3::set(const double _x, const double _y, const double _z) {
    x = _x;
    y = _y;
    z = _z;
}

void Vec3::normalize() {
    *this = *this/this->norm();
}

double Vec3::norm()const {
    return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::cross(const Vec3 v)const {
    return Vec3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
}

double Vec3::operator*(const Vec3 v)const {
    return v.x*x + v.y*y + v.z*z;
}

Vec3 Vec3::operator*(const double scalar)const {
    return Vec3(x*scalar, y*scalar, z*scalar);
}

Vec3 Vec3::operator-(const Vec3 v)const {
    return Vec3(x-v.x, y-v.y, z-v.z);
}

Vec3 Vec3::operator+(const Vec3 v)const {
    return Vec3(x+v.x, y+v.y, z+v.z);
}

Vec3 Vec3::operator/(const double scalar)const {
    return Vec3(x/scalar, y/scalar, z/scalar);
}


std::string Vec3::str()const {
    std::stringstream out;
    out << "(" << x << " " << y << " " << z << ")m";
    return out.str();
}

void Vec3::mirror(Vec3* ray)const {
    // mirror martix
    //
    // This is taken from
    // (OPTI 421/521 – Introductory Optomechanical Engineering)
    // J.H. Bruge
    // University of Arizona
    //
    //
    //                k1    n     k2
    //                 \    /\   /
    //                  \   |   /
    //                   \  |  /
    //                    \ | /
    // ____________________\|/______________________
    //     mirror-surface
    //
    // k1: incidate ray
    // k2: reflected ray
    // n:  surface normal
    //
    // n = [nx,ny,nz]^T
    //
    // It can be written:
    //
    // k2 = M*k1
    //
    // M = EYE - 2*n*n^T
    //
    // using EYE =  [1 0 0]
    //              [0 1 0]
    //              [0 0 1]
    //
    // This function has to be called like:
    //
    // Vec3 vec_surface_normal;
    // Vec3 dir_of_ray_to_be_reflected;
    //
    // vec_surface_normal.mirror(dir_of_ray_to_be_reflected);
    //
    // dir_of_ray_to_be_reflected is overwritten with the reflected ray.

    ray->set(   // X
                (1. - 2.*x*x) * ray->x +
                    - 2.*x*y  * ray->y +
                    - 2.*x*z  * ray->z,
                // Y
                    - 2.*x*y  * ray->x +
                (1. - 2.*y*y) * ray->y +
                    - 2.*y*z  * ray->z,
                // Z
                    - 2.*x*z  * ray->x +
                    - 2.*y*z  * ray->y +
                (1. - 2.*z*z) * ray->z);
}

double Vec3::angle_in_between(const Vec3& that)const {
    Vec3 this_normalized = *this/this->norm();
    Vec3 that_normalized = that/that.norm();
    return acos(this_normalized*that_normalized);
}

bool Vec3::operator == (const Vec3& eqVec)const {
    return squared_distance_to(eqVec) <= MAX_DEVIATION_EQUAL_VEC3_SQUARE;
}

bool Vec3::operator != (const Vec3& eqVec)const {
    return squared_distance_to(eqVec) > MAX_DEVIATION_EQUAL_VEC3_SQUARE;
}

double Vec3::squared_distance_to(const Vec3 &v)const {
    const Vec3 diff = (*this-v);
    return diff*diff;
}

double Vec3::distance_to(const Vec3 &v)const {
    return (*this-v).norm();
}

bool Vec3::is_paralell_to_z_axis()const {
    return x == 0. && y == 0. && fabs(z) > 0.;
}

bool Vec3::is_parallel_to_x_y_plane()const {
    return z == 0. && ( x != 0. || y != 0. );
}

bool Vec3::norm_is_less_equal_than(const double length_to_compare)const {
    // avoid the sqrt for speed up
    return (*this)*(*this) <= length_to_compare*length_to_compare;
}

unsigned int Vec3::octant()const {
    // encodes the octant sectors where the vector is pointing to
    // x y z sector
    // - - -   0
    // - - +   1
    // - + -   2
    // - + +   3
    // + - -   4
    // + - +   5
    // + + -   6
    // + + +   7
    const bool sx = x >= 0.;
    const bool sy = y >= 0.;
    const bool sz = z >= 0.;
    return 4*sx + 2*sy + 1*sz;
}

Vec3 Vec3::project_in_x_z_plane()const {
    return Vec3(x, 0., z);
}

Vec3 Vec3::project_in_y_z_plane()const {
    return Vec3(0., y, z);
}

Vec3 Vec3::project_in_x_y_plane()const {
    return Vec3(x, y, 0.);
}

}  // namespace merlict
