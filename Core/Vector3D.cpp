#include "Vector3D.h"
//------------------------------------------------------------------------------
const Vector3D Vector3D::null   = Vector3D(0.0, 0.0, 0.0);
const Vector3D Vector3D::unit_x = Vector3D(1.0, 0.0, 0.0);
const Vector3D Vector3D::unit_y = Vector3D(0.0, 1.0, 0.0);
const Vector3D Vector3D::unit_z = Vector3D(0.0, 0.0, 1.0);
const double Vector3D::max_deviation_of_eq_vectors = 1e-7;
//------------------------------------------------------------------------------
Vector3D::Vector3D(){
}
//------------------------------------------------------------------------------
Vector3D::Vector3D(const double nx,const double ny,const double nz) {
	X=nx; 
	Y=ny; 
	Z=nz;
}
//------------------------------------------------------------------------------
void Vector3D::set(const double nx,const double ny,const double nz) {
	X=nx; 
	Y=ny; 
	Z=nz;
}
//------------------------------------------------------------------------------
void Vector3D::normalize() {
	*this = *this/this->norm();
}
//------------------------------------------------------------------------------
double Vector3D::norm()const {
	return sqrt( X*X + Y*Y + Z*Z );
}
//------------------------------------------------------------------------------
Vector3D Vector3D::cross(const Vector3D v)const {
	return Vector3D(Y*v.Z-Z*v.Y, Z*v.X-X*v.Z, X*v.Y-Y*v.X);
}
//------------------------------------------------------------------------------
double Vector3D::operator*(const Vector3D vec_two)const {
	return vec_two.X*X + vec_two.Y*Y + vec_two.Z*Z;
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator*(const double scalar)const {
	return Vector3D(X*scalar, Y*scalar, Z*scalar);
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator-(const Vector3D vec)const {
	return Vector3D(X-vec.X, Y-vec.Y, Z-vec.Z);
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator+(const Vector3D vec)const {
	return Vector3D(X+vec.X, Y+vec.Y, Z+vec.Z);
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator/(const double scalar)const {
	return Vector3D(X/scalar, Y/scalar, Z/scalar);
}
//------------------------------------------------------------------------------
void Vector3D::operator=(const Vector3D eq) {
	X = eq.X;
	Y = eq.Y;
	Z = eq.Z;
}
//------------------------------------------------------------------------------
std::string Vector3D::get_print()const {
	std::stringstream out; 
	out << "(" << X << " " << Y << " " << Z << ")m";
	return out.str();
}
//------------------------------------------------------------------------------
void Vector3D::mirror(Vector3D* ray)const {
	// mirror martix
	//
	// This is taken from 
	// (OPTI 421/521 – Introductory Optomechanical Engineering)
	// J.H. Bruge
	// University of Arizona
	//
	//
	//                k1    n     k2
	//	               \    /\   /
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
	//				[0 1 0]
	//              [0 0 1]
	//
	// This function has to be called like:
	//
	// Vector3D vec_surface_normal;
	// Vector3D dir_of_ray_to_be_reflected;
	//
	// vec_surface_normal.mirror(dir_of_ray_to_be_reflected);
	//
	// dir_of_ray_to_be_reflected is overwritten with the reflected ray.

	ray->set(
		//X
		(1.0 - 2.0*X*X) * ray->X +
		     - 2.0*X*Y  * ray->Y +
		     - 2.0*X*Z  * ray->Z,
		//Y
		     - 2.0*X*Y  * ray->X +
		(1.0 - 2.0*Y*Y) * ray->Y +
		     - 2.0*Y*Z  * ray->Z,
		//Z
		     - 2.0*X*Z  * ray->X + 
		     - 2.0*Y*Z  * ray->Y + 
		(1.0 - 2.0*Z*Z) * ray->Z
	);
}
//------------------------------------------------------------------------------
double Vector3D::get_angle_in_between_in_rad(const Vector3D& that)const {
	Vector3D this_normalized = *this/this->norm();
	Vector3D that_normalized = that/that.norm(); 
	return acos(this_normalized*that_normalized);
}
//------------------------------------------------------------------------------
double Vector3D::x() const{
	return X;
}
//------------------------------------------------------------------------------
double Vector3D::y() const{
	return Y;
}
//------------------------------------------------------------------------------
double Vector3D::z() const{
	return Z;
}
//------------------------------------------------------------------------------
bool Vector3D::operator == (const Vector3D& eqVec) const{
	return distance_to(eqVec) <= max_deviation_of_eq_vectors;
}
//------------------------------------------------------------------------------
bool Vector3D::operator != (const Vector3D& eqVec) const{
	return distance_to(eqVec) > max_deviation_of_eq_vectors;
}
//------------------------------------------------------------------------------
double Vector3D::distance_to(const Vector3D &v)const{
	return (*this-v).norm();
}
//------------------------------------------------------------------------------
bool Vector3D::is_paralell_to_z_axis()const{
	return X == 0.0 && Y == 0.0 && Z > 0.0;
}
//------------------------------------------------------------------------------
bool Vector3D::is_parallel_to_x_y_plane()const{
	return Z == 0.0 && ( X != 0.0 || Y != 0.0 );
}
//------------------------------------------------------------------------------
bool Vector3D::norm_is_less_equal_than(const double length_to_compare)const {
	// avoid the sqrt for speed up
	return (*this)*(*this) <= length_to_compare*length_to_compare;
}
//------------------------------------------------------------------------------
uint Vector3D::get_octant()const {
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
	const bool sx = X >= 0.0;
	const bool sy = Y >= 0.0;
	const bool sz = Z >= 0.0;
	return 4*sx + 2*sy + 1*sz;
}
//------------------------------------------------------------------------------
Vector3D Vector3D::get_projection_on_XZ_plane()const {
	return Vector3D(X, 0.0, Z);
}
//------------------------------------------------------------------------------
Vector3D Vector3D::get_projection_on_YZ_plane()const {
	return Vector3D(0.0, Y, Z);
}
//------------------------------------------------------------------------------
Vector3D Vector3D::get_projection_on_XY_plane()const {
	return Vector3D(X, Y, 0.0);
}