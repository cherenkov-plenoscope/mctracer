#include "Intersection.h"
//------------------------------------------------------------------------------
Intersection::Intersection() {
	intersecting_object = nullptr;
}
//------------------------------------------------------------------------------
Intersection::Intersection(
	const CartesianFrame* intersectiong_object,
	const Vector3D intersection_vector,
	const Vector3D surfacenormal_in_intersection_point,
	const double distance_of_ray_support_to_intersection
) {
	this->intersecting_object = intersectiong_object;
	intersection_point = intersection_vector;
	this->surfacenormal_in_intersection_point = surfacenormal_in_intersection_point;
	distance_of_ray_in_m = distance_of_ray_support_to_intersection;
}
//------------------------------------------------------------------------------
bool Intersection::does_intersect()const {
	return intersecting_object != nullptr;
}
//------------------------------------------------------------------------------
const CartesianFrame * Intersection::get_intersecting_object()const {
	return intersecting_object;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_intersection_vector_in_object_system()const {
	return intersection_point;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_intersection_vector_in_world_system()const {
	return intersecting_object->
		frame2world()->get_transformed_position(intersection_point);
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_surface_normal_in_object_system()const {
	return surfacenormal_in_intersection_point;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_surface_normal_in_world_system()const {
	return intersecting_object->
		frame2world()->get_transformed_orientation(surfacenormal_in_intersection_point);	
}
//------------------------------------------------------------------------------
double Intersection::get_intersection_distance()const {
	return distance_of_ray_in_m;
}
//------------------------------------------------------------------------------
std::string Intersection::get_print()const {

	std::stringstream out; 
	if(does_intersect()) {
		out << intersecting_object->get_name_of_frame() << " in ";
		out << get_intersection_vector_in_world_system() << " after ";
		out << get_intersection_distance() << "m";
	}else
		out << "no intersection with any object";
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
void Intersection::get_reflection_direction_in_object_system(Vector3D* vec)const {
	surfacenormal_in_intersection_point.mirror(vec);
}