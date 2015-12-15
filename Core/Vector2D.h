//=================================
// include guard
#ifndef __VECTOR2D_H_INCLUDED__
#define __VECTOR2D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>
#include "Core/Printable.h"

//=================================
class Vector2D : public Printable {
private:

	double X;
	double Y;
	static const double max_deviation_of_eq_vectors;
public:
	static const Vector2D null;
	static const Vector2D unit_x;
	static const Vector2D unit_y;

	Vector2D();
	Vector2D(const double nx, const double ny);
	void set(const double nx, const double ny);
	double x()const;
	double y()const;
	uint get_quadrant()const;
	double norm()const;
	void normalize();
	double get_angle_in_between_in_rad(const Vector2D& vec)const;
	double operator*(const Vector2D vec_two)const;
	Vector2D operator*(const double scalar)const;
	Vector2D operator-(const Vector2D vec_two)const;
	Vector2D operator+(const Vector2D vec_two)const;
	Vector2D operator/(const double scalar)const;
	void operator=(const Vector2D eq);
	bool operator == (const Vector2D& eqVec)const;
	bool operator != (const Vector2D& eqVec)const;
	double distance_to(const Vector2D &v)const;
	bool norm_is_less_equal_than(const double length_to_compare)const;
	std::string get_print()const;
};
#endif // __VECTOR2D_H_INCLUDED__ 
