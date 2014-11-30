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
#include "CsvHandler.h"
#include "GlobalSettings.h"
//=================================
class Vector3D {
private:
	// in meters
	double X;
	double Y;
	double Z;
public:
//------------------------------------------------------------------------------
// SET
Vector3D();
Vector3D(const double nx,const double ny,const double nz);
void set(const double nx,const double ny,const double nz);
//------------------------------------------------------------------------------
// GET
double x() const;
double y() const;
double z() const;
//------------------------------------------------------------------------------
// Functionality
double norm2() const;
void normalize();
Vector3D CrossProduct(const Vector3D v) const;
void mirror(Vector3D* ray) const;
void set_unit_vector_x();
void set_unit_vector_y();
void set_unit_vector_z();
void set_null_vector();
//------------------------------------------------------------------------------
// Operators
double operator*(const Vector3D vec_two) const;
Vector3D operator*(const double scalar) const;
Vector3D operator-(const Vector3D vec_two) const;
Vector3D operator+(const Vector3D vec_two) const;
Vector3D operator/(const double scalar) const;
void operator=(const Vector3D eq);
bool operator == (const Vector3D& eqVec) const;
bool operator != (const Vector3D& eqVec) const;
double distance_to(const Vector3D &v)const;
bool is_paralell_to_z_axis()const;
bool is_parallel_to_x_y_plane()const;
//------------------------------------------------------------------------------
// Printing
void disp() const;
std::string get_string() const;
CsvRow getCsvRow(GlobalSettings& settings) const;
//------------------------------------------------------------------------------
// ostream
friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec);
};

#endif // __VECTOR3D_H_INCLUDED__ 
