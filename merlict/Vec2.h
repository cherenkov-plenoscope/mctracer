// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_VEC2_H_
#define MERLICT_VEC2_H_

#include <string>

namespace merlict {

struct Vec2 {
    double x;
    double y;

    Vec2();
    Vec2(const double x, const double y);
    void set(const double x, const double y);
    unsigned int get_quadrant()const;
    double norm()const;
    void normalize();
    double angle_in_between(const Vec2& vec)const;
    double operator*(const Vec2 v)const;
    Vec2 operator*(const double scalar)const;
    Vec2 operator-(const Vec2 v)const;
    Vec2 operator+(const Vec2 v)const;
    Vec2 operator/(const double scalar)const;
    void operator=(const Vec2 eq);
    bool operator == (const Vec2& eq_v)const;
    bool operator != (const Vec2& eq_v)const;
    double distance_to(const Vec2 &v)const;
    bool norm_is_less_equal_than(const double length_to_compare)const;
    std::string str()const;
};

const Vec2 VEC2_ORIGIN = Vec2(0.0, 0.0);
const Vec2 VEC2_UNIT_X = Vec2(1.0, 0.0);
const Vec2 VEC2_UNIT_Y = Vec2(0.0, 1.0);
const double MAX_DEVIATION_EQUAL_VEC2 = 1e-7;

}  // namespace merlict

#endif  // MERLICT_VEC2_H_
