#include "Ray.h"
#include <sstream>
//------------------------------------------------------------------------------
Ray::Ray() {}
//------------------------------------------------------------------------------
Ray::Ray(const Vector3D support, const Vector3D direction) {
	SetRay(support, direction);
}
//------------------------------------------------------------------------------
void Ray::SetRay(const Vector3D support, const Vector3D direction) {
	SetSupport(support);
	SetDirection(direction);
}
//------------------------------------------------------------------------------
void Ray::SetSupport(const Vector3D sup) {
	support = sup;
}
//------------------------------------------------------------------------------
void Ray::SetDirection(const Vector3D dir) {
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
Vector3D Ray::PositionOnRay(const double scalar)const {
	return support + direction*scalar;
}
//------------------------------------------------------------------------------
Vector3D Ray::Support()const {
	return support;
}
//------------------------------------------------------------------------------
Vector3D Ray::Direction()const {
	return direction;
}
//------------------------------------------------------------------------------
void Ray::transform(const HomoTrafo3D *T) {
	T->transform_position(&support);
	T->transform_orientation(&direction);	
}
//------------------------------------------------------------------------------
double Ray::get_parameter_on_ray_for_closest_distance_to_point(
	const Vector3D &point
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
	double d = direction*point;
	
	// Insert the ray into plane equation and solve for the ray parameter.
	// The ray's direction is normalized, therefore: (direction * direction)=1
	return d - support*direction;
}
//------------------------------------------------------------------------------
double Ray::get_closest_distance_to_point(const Vector3D &point)const {
	const double a = get_parameter_on_ray_for_closest_distance_to_point(point);
	const Vector3D q = PositionOnRay(a);
	const Vector3D shortest_connection = point - q;
	return shortest_connection.norm();
}
//------------------------------------------------------------------------------
Ray Ray::get_ray_transformed_in_object_system_of(const Frame* frame)const {
	Ray ray_in_object_system_of_frame = *this;
	ray_in_object_system_of_frame.transform(frame->world2frame());
	return ray_in_object_system_of_frame;
}
