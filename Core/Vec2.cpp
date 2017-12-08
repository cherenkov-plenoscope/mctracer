// Copyright 2014 Sebastian A. Mueller
#include "Core/Vec2.h"
#include <math.h>
#include <sstream>


const Vec2 Vec2::null   = Vec2(0.0, 0.0);
const Vec2 Vec2::unit_x = Vec2(1.0, 0.0);
const Vec2 Vec2::unit_y = Vec2(0.0, 1.0);
const double Vec2::max_deviation_of_eq_vectors = 1e-7;

Vec2::Vec2() {}

Vec2::Vec2(const double nx, const double ny) {
    set(nx, ny);
}

void Vec2::set(const double nx, const double ny) {
    X = nx;
    Y = ny;
}

void Vec2::normalize() {
    *this = *this/this->norm();
}

double Vec2::norm()const {
    return sqrt( X*X + Y*Y );
}

double Vec2::operator*(const Vec2 vec)const {
    return vec.X*X + vec.Y*Y;
}

Vec2 Vec2::operator*(const double scalar)const {
    return Vec2(X*scalar, Y*scalar);
}

Vec2 Vec2::operator-(const Vec2 vec)const {
    return Vec2(X-vec.X, Y-vec.Y);
}

Vec2 Vec2::operator+(const Vec2 vec)const {
    return Vec2(X+vec.X, Y+vec.Y);
}

Vec2 Vec2::operator/(const double scalar)const {
    return Vec2(X/scalar, Y/scalar);
}

void Vec2::operator=(const Vec2 eq) {
    X = eq.X;
    Y = eq.Y;
}

std::string Vec2::str()const {
    std::stringstream out;
    out << "(" << X << " " << Y << ")m";
    return out.str();
}

double Vec2::get_angle_in_between_in_rad(const Vec2& that)const {
    Vec2 this_normalized = *this/this->norm();
    Vec2 that_normalized = that/that.norm();
    return acos(this_normalized*that_normalized);
}

double Vec2::x()const {
    return X;
}

double Vec2::y()const {
    return Y;
}

bool Vec2::operator == (const Vec2& eqVec) const {
    return distance_to(eqVec) <= max_deviation_of_eq_vectors;
}

bool Vec2::operator != (const Vec2& eqVec) const {
    return distance_to(eqVec) > max_deviation_of_eq_vectors;
}

double Vec2::distance_to(const Vec2 &v)const {
    return (*this-v).norm();
}

bool Vec2::norm_is_less_equal_than(const double length_to_compare)const {
    // avoid the sqrt for speed up
    return (*this)*(*this) <= length_to_compare*length_to_compare;
}

unsigned int Vec2::get_quadrant()const {
    // encodes the octant sectors where the vector is pointing to
    // x y sector
    // - -   0
    // - +   1
    // + -   2
    // + +   3
    const bool sx = X > 0.0;
    const bool sy = Y > 0.0;
    return 2*sx + 1*sy;
}
