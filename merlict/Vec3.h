// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_VEC3_H_
#define MERLICT_VEC3_H_

#include <string>

namespace merlict {

struct Vec3 {
    double x;
    double y;
    double z;

    Vec3();
    Vec3(const double x, const double y, const double z);
    void set(const double x, const double y, const double z);
    unsigned int octant()const;
    Vec3 project_in_x_z_plane()const;
    Vec3 project_in_y_z_plane()const;
    Vec3 project_in_x_y_plane()const;
    double norm()const;
    void normalize();
    Vec3 cross(const Vec3 v)const;
    void mirror(Vec3* ray)const;
    double angle_in_between(const Vec3& vec)const;
    double operator*(const Vec3 v)const;
    Vec3 operator*(const double scalar)const;
    Vec3 operator-(const Vec3 v)const;
    Vec3 operator+(const Vec3 v)const;
    Vec3 operator/(const double scalar)const;
    bool operator == (const Vec3& eqVec)const;
    bool operator != (const Vec3& eqVec)const;
    double distance_to(const Vec3 &v)const;
    double squared_distance_to(const Vec3 &v)const;
    bool is_paralell_to_z_axis()const;
    bool is_parallel_to_x_y_plane()const;
    bool norm_is_less_equal_than(const double length_to_compare)const;
    std::string str()const;
};

const Vec3 VEC3_ORIGIN = Vec3(0., 0., 0.);
const Vec3 VEC3_UNIT_X = Vec3(1., 0., 0.);
const Vec3 VEC3_UNIT_Y = Vec3(0., 1., 0.);
const Vec3 VEC3_UNIT_Z = Vec3(0., 0., 1.);
const double MAX_DEVIATION_EQUAL_VEC3_SQUARE = 1e-14;

}  // namespace merlict

#endif  // MERLICT_VEC3_H_
