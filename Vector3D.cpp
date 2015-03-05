#include "Vector3D.h"
//------------------------------------------------------------------------------
Vector3D::Vector3D(){
}
//------------------------------------------------------------------------------
Vector3D::Vector3D(const double nx,const double ny,const double nz){
	X=nx; 
	Y=ny; 
	Z=nz;
}
//------------------------------------------------------------------------------
void Vector3D::set(const double nx,const double ny,const double nz){
	X=nx; 
	Y=ny; 
	Z=nz;
}
//------------------------------------------------------------------------------
void Vector3D::normalize() {
	*this = *this/this->norm2();
}
//------------------------------------------------------------------------------
double Vector3D::norm2() const{
	return sqrt( X*X + Y*Y + Z*Z );
}
//------------------------------------------------------------------------------
Vector3D Vector3D::CrossProduct(const Vector3D v) const{
// crossproduct
	Vector3D temp;
	temp.X = Y*v.Z - Z*v.Y;
	temp.Y = Z*v.X - X*v.Z;
	temp.Z = X*v.Y - Y*v.X;
	return temp;
}
//------------------------------------------------------------------------------
double Vector3D::operator*(const Vector3D vec_two) const{
	double scalarproduct;
	scalarproduct = vec_two.X*X + vec_two.Y*Y + vec_two.Z*Z;
	return scalarproduct;
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator*(const double scalar) const{
	Vector3D  temp;
	temp.X=X*scalar;
	temp.Y=Y*scalar;
	temp.Z=Z*scalar;
	return temp;
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator-(const Vector3D vec_two) const{
	Vector3D temp;
	temp.X = X-vec_two.X;
	temp.Y = Y-vec_two.Y;
	temp.Z = Z-vec_two.Z;
	return temp;
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator+(const Vector3D vec_two) const{
	Vector3D temp;
	temp.X = X+vec_two.X;
	temp.Y = Y+vec_two.Y;
	temp.Z = Z+vec_two.Z;
	return temp;
}
//------------------------------------------------------------------------------
Vector3D Vector3D::operator/(const double scalar) const{
	Vector3D temp;
	temp.X = X/scalar;
	temp.Y = Y/scalar;
	temp.Z = Z/scalar;
	return temp;
}
//------------------------------------------------------------------------------
void Vector3D::operator=(const Vector3D eq){
	X =eq.X;
	Y =eq.Y;
	Z =eq.Z;
}
//------------------------------------------------------------------------------
void Vector3D::print() const{
	std::cout << get_string();
}
//------------------------------------------------------------------------------
std::string Vector3D::get_string() const{
	std::stringstream out; 
	out << "(" << X << " " << Y << " " << Z << ")m";
	return out.str();
}
//------------------------------------------------------------------------------
void Vector3D::mirror(Vector3D* ray) const{
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
double Vector3D::get_angle_in_between_in_rad(const Vector3D& vec)const {
	Vector3D this_normalized = *this/this->norm2();
	Vector3D that_normalized = vec/vec.norm2(); 
	return acos(this_normalized*that_normalized);
}
//------------------------------------------------------------------------------
void Vector3D::set_unit_vector_x(){
	X = 1.0;
	Y = 0.0;
	Z = 0.0;
}
//------------------------------------------------------------------------------
void Vector3D::set_unit_vector_y(){
	X = 0.0;
	Y = 1.0;
	Z = 0.0;
}
//------------------------------------------------------------------------------
void Vector3D::set_unit_vector_z(){
	X = 0.0;
	Y = 0.0;
	Z = 1.0;
}
//------------------------------------------------------------------------------
void Vector3D::set_null_vector(){
	X = 0.0;
	Y = 0.0;
	Z = 0.0;
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
	return (distance_to(eqVec) < 1e-7) ? true : false;
}
bool Vector3D::operator != (const Vector3D& eqVec) const{
	return (distance_to(eqVec) < 1e-7) ? false : true;
}
//------------------------------------------------------------------------------
double Vector3D::distance_to(const Vector3D &v)const{
	return (*this-v).norm2();
}
//------------------------------------------------------------------------------
bool Vector3D::is_paralell_to_z_axis()const{
	return ( X==0.0 && Y==0.0 && Z>0.0 ) ? true : false;
}
//------------------------------------------------------------------------------
bool Vector3D::is_parallel_to_x_y_plane()const{
	return( Z==0 && ( X!=0.0 || Y!=0.0 ) ) ? true : false;
}
//------------------------------------------------------------------------------
bool Vector3D::norm2_is_less_equal_than(const double length_to_compare)const {
	// avoid the sqrt for speed up
	return (*this)*(*this) <= length_to_compare*length_to_compare;
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Vector3D& vec){
    os << vec.get_string();
    return os;
}
