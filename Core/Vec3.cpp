#include "Vec3.h"
//------------------------------------------------------------------------------
const Vec3 Vec3::null   = Vec3(0.0, 0.0, 0.0);
const Vec3 Vec3::unit_x = Vec3(1.0, 0.0, 0.0);
const Vec3 Vec3::unit_y = Vec3(0.0, 1.0, 0.0);
const Vec3 Vec3::unit_z = Vec3(0.0, 0.0, 1.0);
const double Vec3::squared_max_deviation_of_eq_vectors = 1e-14;// 1e-7;
//------------------------------------------------------------------------------
Vec3::Vec3(){
}
//------------------------------------------------------------------------------
Vec3::Vec3(const double nx,const double ny,const double nz):
	X(nx), Y(ny), Z(nz)
{}
//------------------------------------------------------------------------------
void Vec3::set(const double nx,const double ny,const double nz) {
	X=nx; 
	Y=ny; 
	Z=nz;
}
//------------------------------------------------------------------------------
void Vec3::normalize() {
	*this = *this/this->norm();
}
//------------------------------------------------------------------------------
double Vec3::norm()const {
	return sqrt( X*X + Y*Y + Z*Z );
}
//------------------------------------------------------------------------------
Vec3 Vec3::cross(const Vec3 v)const {
	return Vec3(Y*v.Z-Z*v.Y, Z*v.X-X*v.Z, X*v.Y-Y*v.X);
}
//------------------------------------------------------------------------------
double Vec3::operator*(const Vec3 vec_two)const {
	return vec_two.X*X + vec_two.Y*Y + vec_two.Z*Z;
}
//------------------------------------------------------------------------------
Vec3 Vec3::operator*(const double scalar)const {
	return Vec3(X*scalar, Y*scalar, Z*scalar);
}
//------------------------------------------------------------------------------
Vec3 Vec3::operator-(const Vec3 vec)const {
	return Vec3(X-vec.X, Y-vec.Y, Z-vec.Z);
}
//------------------------------------------------------------------------------
Vec3 Vec3::operator+(const Vec3 vec)const {
	return Vec3(X+vec.X, Y+vec.Y, Z+vec.Z);
}
//------------------------------------------------------------------------------
Vec3 Vec3::operator/(const double scalar)const {
	return Vec3(X/scalar, Y/scalar, Z/scalar);
}
//------------------------------------------------------------------------------
void Vec3::operator=(const Vec3 eq) {
	X = eq.X;
	Y = eq.Y;
	Z = eq.Z;
}
//------------------------------------------------------------------------------
std::string Vec3::str()const {
	std::stringstream out; 
	out << "(" << X << " " << Y << " " << Z << ")m";
	return out.str();
}
//------------------------------------------------------------------------------
void Vec3::mirror(Vec3* ray)const {
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
	// Vec3 vec_surface_normal;
	// Vec3 dir_of_ray_to_be_reflected;
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
double Vec3::get_angle_in_between_in_rad(const Vec3& that)const {
	Vec3 this_normalized = *this/this->norm();
	Vec3 that_normalized = that/that.norm(); 
	return acos(this_normalized*that_normalized);
}
//------------------------------------------------------------------------------
double Vec3::x() const{
	return X;
}
//------------------------------------------------------------------------------
double Vec3::y() const{
	return Y;
}
//------------------------------------------------------------------------------
double Vec3::z() const{
	return Z;
}
//------------------------------------------------------------------------------
bool Vec3::operator == (const Vec3& eqVec) const{
	return squared_distance_to(eqVec) <= squared_max_deviation_of_eq_vectors;
}
//------------------------------------------------------------------------------
bool Vec3::operator != (const Vec3& eqVec) const{
	return squared_distance_to(eqVec) > squared_max_deviation_of_eq_vectors;
}
//------------------------------------------------------------------------------
double Vec3::squared_distance_to(const Vec3 &v)const{
	const Vec3 diff = (*this-v);
	return diff*diff;
}
//------------------------------------------------------------------------------
double Vec3::distance_to(const Vec3 &v)const{
	return (*this-v).norm();
}
//------------------------------------------------------------------------------
bool Vec3::is_paralell_to_z_axis()const{
	return X == 0.0 && Y == 0.0 && Z > 0.0;
}
//------------------------------------------------------------------------------
bool Vec3::is_parallel_to_x_y_plane()const{
	return Z == 0.0 && ( X != 0.0 || Y != 0.0 );
}
//------------------------------------------------------------------------------
bool Vec3::norm_is_less_equal_than(const double length_to_compare)const {
	// avoid the sqrt for speed up
	return (*this)*(*this) <= length_to_compare*length_to_compare;
}
//------------------------------------------------------------------------------
uint Vec3::get_octant()const {
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
Vec3 Vec3::get_projection_on_XZ_plane()const {
	return Vec3(X, 0.0, Z);
}
//------------------------------------------------------------------------------
Vec3 Vec3::get_projection_on_YZ_plane()const {
	return Vec3(0.0, Y, Z);
}
//------------------------------------------------------------------------------
Vec3 Vec3::get_projection_on_XY_plane()const {
	return Vec3(X, Y, 0.0);
}