#include "Ray.h"
#include <sstream>
//------------------------------------------------------------------------------
Ray::Ray() {}
//------------------------------------------------------------------------------
Ray::Ray(const Vec3 support, const Vec3 direction) {
	SetRay(support, direction);
}
//------------------------------------------------------------------------------
void Ray::SetRay(const Vec3 support, const Vec3 direction) {
	SetSupport(support);
	SetDirection(direction);
}
//------------------------------------------------------------------------------
void Ray::SetSupport(const Vec3 sup) {
	support = sup;
}
//------------------------------------------------------------------------------
void Ray::SetDirection(const Vec3 dir) {
	direction = dir;
	direction.normalize();
}
//------------------------------------------------------------------------------
string Ray::get_print()const {
	stringstream out;
	out << "support: " << support << ", direction: " << direction;
	return out.str();
}
//------------------------------------------------------------------------------
Vec3 Ray::PositionOnRay(const double scalar)const {
	return support + direction*scalar;
}
//------------------------------------------------------------------------------
Vec3 Ray::Support()const {
	return support;
}
//------------------------------------------------------------------------------
Vec3 Ray::Direction()const {
	return direction;
}
//------------------------------------------------------------------------------
void Ray::transform(const HomTra3 *T) {
	T->transform_position(&support);
	T->transform_orientation(&direction);	
}
//------------------------------------------------------------------------------
double Ray::get_parameter_on_ray_for_closest_distance_to_point(
	const Vec3 &point
)const {
	// We create a plane orthogonal to this ray and containing the point
	// plane equation:
	// 	d = x*a + y*b + z*c
	//
	// We set the normal vector n of the plane to the ray's direction vector:
	//  a=direction.x b=direction.y c=direction.z
	//
	// Now we insert the support vector of the frame into the plane eqaution:
	//  d = point.x*dirx + point.y*diry + point.z*dirz
	const double d = direction*point;
	
	// Insert the ray into plane equation and solve for the ray parameter.
	// The ray's direction is normalized, therefore: (direction * direction)=1
	return d - support*direction;
}
//------------------------------------------------------------------------------
double Ray::get_closest_distance_to_point(const Vec3 &point)const {
	const double a = get_parameter_on_ray_for_closest_distance_to_point(point);
	const Vec3 q = PositionOnRay(a);
	const Vec3 shortest_connection = point - q;
	return shortest_connection.norm();
}
//------------------------------------------------------------------------------
Ray Ray::get_ray_transformed_in_object_system_of(const Frame* frame)const {
	Ray ray_in_object_system_of_frame = *this;
	ray_in_object_system_of_frame.transform(frame->world2frame());
	return ray_in_object_system_of_frame;
}
