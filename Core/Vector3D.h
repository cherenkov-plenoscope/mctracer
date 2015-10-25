//=================================
// include guard
#ifndef __VECTOR3D_H_INCLUDED__
#define __VECTOR3D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>

//=================================
class Vector3D {
private:

	// in meters
	double X;
	double Y;
	double Z;
	static const double max_deviation_of_eq_vectors;
public:
	static const Vector3D null;
	static const Vector3D unit_x;
	static const Vector3D unit_y;
	static const Vector3D unit_z;

	Vector3D();
	Vector3D(const double nx, const double ny, const double nz);
	void set(const double nx, const double ny, const double nz);
	double x()const;
	double y()const;
	double z()const;
	uint get_octant()const;
	double norm()const;
	void normalize();
	Vector3D cross(const Vector3D v)const;
	void mirror(Vector3D* ray)const;
	double get_angle_in_between_in_rad(const Vector3D& vec)const;
	double operator*(const Vector3D vec_two)const;
	Vector3D operator*(const double scalar)const;
	Vector3D operator-(const Vector3D vec_two)const;
	Vector3D operator+(const Vector3D vec_two)const;
	Vector3D operator/(const double scalar)const;
	void operator=(const Vector3D eq);
	bool operator == (const Vector3D& eqVec)const;
	bool operator != (const Vector3D& eqVec)const;
	double distance_to(const Vector3D &v)const;
	bool is_paralell_to_z_axis()const;
	bool is_parallel_to_x_y_plane()const;
	bool norm_is_less_equal_than(const double length_to_compare)const;
	std::string get_print()const;
	friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec);
};
#endif // __VECTOR3D_H_INCLUDED__ 
