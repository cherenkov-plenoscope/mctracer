// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_VEC2_H_
#define MCTRACER_CORE_VEC2_H_

#include <string>

class Vec2 {
 private:
    double X;
    double Y;
    static const double max_deviation_of_eq_vectors;

 public:
    static const Vec2 ORIGIN;
    static const Vec2 UNIT_X;
    static const Vec2 UNIT_Y;

    Vec2();
    Vec2(const double nx, const double ny);
    void set(const double nx, const double ny);
    double x()const;
    double y()const;
    unsigned int get_quadrant()const;
    double norm()const;
    void normalize();
    double get_angle_in_between_in_rad(const Vec2& vec)const;
    double operator*(const Vec2 vec_two)const;
    Vec2 operator*(const double scalar)const;
    Vec2 operator-(const Vec2 vec_two)const;
    Vec2 operator+(const Vec2 vec_two)const;
    Vec2 operator/(const double scalar)const;
    void operator=(const Vec2 eq);
    bool operator == (const Vec2& eqVec)const;
    bool operator != (const Vec2& eqVec)const;
    double distance_to(const Vec2 &v)const;
    bool norm_is_less_equal_than(const double length_to_compare)const;
    std::string str()const;
};
#endif  // MCTRACER_CORE_VEC2_H_
