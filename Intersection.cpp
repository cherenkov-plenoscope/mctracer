#include "Intersection.h"
//------------------------------------------------------------------------------
Intersection::Intersection() {
	intersecting_object = nullptr;
	has_intersection = false;
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
Vector3D Intersection::get_surface_normal_in_object_system()const {
	return surfacenormal_in_intersection_point;
}
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
const CartesianFrame * Intersection::get_pointer_to_intersecting_object()const {
	return intersecting_object;
}
//------------------------------------------------------------------------------
void Intersection::set_pointer_to_intersecting_object(
	const CartesianFrame* new_ptr_to_intersectiong_object
){
	intersecting_object=new_ptr_to_intersectiong_object;
}
//------------------------------------------------------------------------------
void Intersection::set_intersection_flag(
	const bool flag_new_intersection
){
	has_intersection = flag_new_intersection;
}
//------------------------------------------------------------------------------
void Intersection::set_intersection(
	const Vector3D* ptr_to_intersection_vector,
	const Vector3D* ptr_to_intersection_object_surface_normal_vector,
	const double* ptr_to_distance_of_ray 
){
		
	intersection_point = *ptr_to_intersection_vector;
	
	surfacenormal_in_intersection_point= 
	*ptr_to_intersection_object_surface_normal_vector;
	
	distance_of_ray_in_m = 
	*ptr_to_distance_of_ray;
}
//------------------------------------------------------------------------------
bool Intersection::get_intersection_flag()const{
	return has_intersection;
}
//------------------------------------------------------------------------------
void Intersection::disp() const{
	std::stringstream out; 
	out << "Intersection:\n";
	out << get_string();
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string Intersection::get_string() const{
	std::stringstream out; 
	out << "has_intersection: ";
	if(has_intersection) {
		out << "true\n";
		out << "intersection vector: ";
		out << intersection_point.get_string() << "\n";
		out << "intersection object surface normal: ";
		out << surfacenormal_in_intersection_point.get_string() << "\n";
		out << "Distance passed by ray: ";
		out << distance_of_ray_in_m << " [m]"<< "\n";
		out << "Object: ";
		if( intersecting_object == NULL ){
			out <<"NULL";
		}else{
			out << intersecting_object->get_name_of_frame();
		}
	}else{
		out << "false\n"; 
	}
	return out.str();
}
//------------------------------------------------------------------------------
double Intersection::get_intersection_distance()const{
	return distance_of_ray_in_m;
}
//------------------------------------------------------------------------------
void Intersection::get_reflection_direction_in_object_system(
	Vector3D* vec
)const{
	surfacenormal_in_intersection_point.mirror(vec);
}
//------------------------------------------------------------------------------
void Intersection::get_intersection_vec_in_object_system(
	Vector3D *inter
)const{ 	
	*inter = intersection_point;
}
//------------------------------------------------------------------------------