#include "Ray.h"
//------------------------------------------------------------------------------
Ray::Ray() {
}
//------------------------------------------------------------------------------
Ray::Ray(const Vector3D support, const Vector3D direction) {
	SetRay(support, direction);
}
//------------------------------------------------------------------------------
void Ray::SetRay(const Vector3D nsup, const Vector3D ndir) {
	support = nsup;
	direction  = ndir;
	direction.normalize();
}
//------------------------------------------------------------------------------
void Ray::SetSupport(const Vector3D nsup) {
	support = nsup;
}
//------------------------------------------------------------------------------
void Ray::SetDirection(const Vector3D ndir) {
	direction = ndir;
	direction.normalize();
}
//------------------------------------------------------------------------------
std::string Ray::get_print()const {
	std::stringstream out;
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
	// Now we insert the support vevtor of the frame into the plane eqaution:
	//  d = point.x*dirx + point.y*diry + point.z*dirz
	double d = direction * point;
	
	// Insert the ray into plane equation and solve for the ray parameter
	return (d - support * direction) / (direction * direction);
}
//------------------------------------------------------------------------------
double Ray::get_distance_to_point_from_position_of_ray_at(
	const Vector3D &point, const double ray_parameter_for_position_on_ray
)const {
	// Calculate point Q. Q is on the ray and in the plane. So Q is the 
	// closest point on the ray to the center of the frame
	Vector3D Q = PositionOnRay(ray_parameter_for_position_on_ray);

	// Calculate the connection vector between Q and and the center of the
	// frame called W
	Vector3D W = point - Q;
	
	// Calculate length of W. The lenght of W is the closest distance
	// between ray and object
	return W.norm();  
}
//------------------------------------------------------------------------------
double Ray::get_closest_distance_to_point(const Vector3D &point)const {
	return get_distance_to_point_from_position_of_ray_at(
		point, 
		get_parameter_on_ray_for_closest_distance_to_point(point)
	);
}
//------------------------------------------------------------------------------
Ray Ray::get_ray_transformed_in_object_system_of(const Frame* frame)const {
	Ray ray_in_object_system_of_frame = *this;
	ray_in_object_system_of_frame.transform(frame->world2frame());
	return ray_in_object_system_of_frame;
}
