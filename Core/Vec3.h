//=================================
// include guard
#ifndef __Vec3_H_INCLUDED__
#define __Vec3_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>
//=================================
class Vec3 {
private:

    double X;
    double Y;
    double Z;
    static const double squared_max_deviation_of_eq_vectors;
public:
    
    static const Vec3 null;
    static const Vec3 unit_x;
    static const Vec3 unit_y;
    static const Vec3 unit_z;

    Vec3();
    Vec3(const double nx, const double ny, const double nz);
    void set(const double nx, const double ny, const double nz);
    double x()const;
    double y()const;
    double z()const;
    uint get_octant()const;
    Vec3 get_projection_on_XZ_plane()const;
    Vec3 get_projection_on_YZ_plane()const;
    Vec3 get_projection_on_XY_plane()const;
    double norm()const;
    void normalize();
    Vec3 cross(const Vec3 v)const;
    void mirror(Vec3* ray)const;
    double get_angle_in_between_in_rad(const Vec3& vec)const;
    double operator*(const Vec3 vec_two)const;
    Vec3 operator*(const double scalar)const;
    Vec3 operator-(const Vec3 vec_two)const;
    Vec3 operator+(const Vec3 vec_two)const;
    Vec3 operator/(const double scalar)const;
    void operator=(const Vec3 eq);
    bool operator == (const Vec3& eqVec)const;
    bool operator != (const Vec3& eqVec)const;
    double distance_to(const Vec3 &v)const;
    double squared_distance_to(const Vec3 &v)const;
    bool is_paralell_to_z_axis()const;
    bool is_parallel_to_x_y_plane()const;
    bool norm_is_less_equal_than(const double length_to_compare)const;
    std::string get_print()const;

    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
        os << vec.get_print();
        return os;
    };
};
#endif // __Vec3_H_INCLUDED__ 
