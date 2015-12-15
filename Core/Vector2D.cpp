#include "Vector2D.h"
//------------------------------------------------------------------------------
const Vector2D Vector2D::null   = Vector2D(0.0, 0.0);
const Vector2D Vector2D::unit_x = Vector2D(1.0, 0.0);
const Vector2D Vector2D::unit_y = Vector2D(0.0, 1.0);
const double Vector2D::max_deviation_of_eq_vectors = 1e-7;
//------------------------------------------------------------------------------
Vector2D::Vector2D(){
}
//------------------------------------------------------------------------------
Vector2D::Vector2D(const double nx,const double ny) {
	set(nx, ny);
}
//------------------------------------------------------------------------------
void Vector2D::set(const double nx,const double ny) {
	X=nx; 
	Y=ny; 
}
//------------------------------------------------------------------------------
void Vector2D::normalize() {
	*this = *this/this->norm();
}
//------------------------------------------------------------------------------
double Vector2D::norm()const {
	return sqrt( X*X + Y*Y );
}
//------------------------------------------------------------------------------
double Vector2D::operator*(const Vector2D vec)const {
	return vec.X*X + vec.Y*Y;
}
//------------------------------------------------------------------------------
Vector2D Vector2D::operator*(const double scalar)const {
	return Vector2D(X*scalar, Y*scalar);
}
//------------------------------------------------------------------------------
Vector2D Vector2D::operator-(const Vector2D vec)const {
	return Vector2D(X-vec.X, Y-vec.Y);
}
//------------------------------------------------------------------------------
Vector2D Vector2D::operator+(const Vector2D vec)const {
	return Vector2D(X+vec.X, Y+vec.Y);
}
//------------------------------------------------------------------------------
Vector2D Vector2D::operator/(const double scalar)const {
	return Vector2D(X/scalar, Y/scalar);
}
//------------------------------------------------------------------------------
void Vector2D::operator=(const Vector2D eq) {
	X = eq.X;
	Y = eq.Y;
}
//------------------------------------------------------------------------------
std::string Vector2D::get_print()const {
	std::stringstream out; 
	out << "(" << X << " " << Y << ")m";
	return out.str();
}
//------------------------------------------------------------------------------
double Vector2D::get_angle_in_between_in_rad(const Vector2D& that)const {
	Vector2D this_normalized = *this/this->norm();
	Vector2D that_normalized = that/that.norm(); 
	return acos(this_normalized*that_normalized);
}
//------------------------------------------------------------------------------
double Vector2D::x() const{
	return X;
}
//------------------------------------------------------------------------------
double Vector2D::y() const{
	return Y;
}
//------------------------------------------------------------------------------
bool Vector2D::operator == (const Vector2D& eqVec) const{
	return distance_to(eqVec) <= max_deviation_of_eq_vectors;
}
//------------------------------------------------------------------------------
bool Vector2D::operator != (const Vector2D& eqVec) const{
	return distance_to(eqVec) > max_deviation_of_eq_vectors;
}
//------------------------------------------------------------------------------
double Vector2D::distance_to(const Vector2D &v)const{
	return (*this-v).norm();
}
//------------------------------------------------------------------------------
bool Vector2D::norm_is_less_equal_than(const double length_to_compare)const {
	// avoid the sqrt for speed up
	return (*this)*(*this) <= length_to_compare*length_to_compare;
}
//------------------------------------------------------------------------------
uint Vector2D::get_quadrant()const {
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