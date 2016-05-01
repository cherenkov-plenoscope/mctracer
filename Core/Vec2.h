//=================================
// include guard
#ifndef __Vec2_H_INCLUDED__
#define __Vec2_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>
//=================================
class Vec2 {
private:

	double X;
	double Y;
	static const double max_deviation_of_eq_vectors;
public:
	static const Vec2 null;
	static const Vec2 unit_x;
	static const Vec2 unit_y;

	Vec2();
	Vec2(const double nx, const double ny);
	void set(const double nx, const double ny);
	double x()const;
	double y()const;
	uint get_quadrant()const;
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
	std::string get_print()const;

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
        os << vec.get_print();
        return os;
    };
};
#endif // __Vec2_H_INCLUDED__ 
