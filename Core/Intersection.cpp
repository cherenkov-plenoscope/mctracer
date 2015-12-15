#include "Intersection.h"
#include <limits>
//------------------------------------------------------------------------------
const Intersection* Intersection::void_intersection = new Intersection();
//------------------------------------------------------------------------------
Intersection::Intersection() {
	object = SurfaceEntity::void_object;
	intersection_point = Vector3D(
		std::numeric_limits<double>::infinity(),
		std::numeric_limits<double>::infinity(),
		std::numeric_limits<double>::infinity()
	);
	surfacenormal_in_intersection_point = Vector3D(0.0,0.0,1.0);
	distance_of_ray_in_m = std::numeric_limits<double>::infinity();
}
//------------------------------------------------------------------------------
Intersection::Intersection(
	const SurfaceEntity* intersectiong_object,
	const Vector3D intersection_vector,
	const Vector3D surfacenormal_in_intersection_point,
	const double distance_of_ray_support_to_intersection,
	const Vector3D incident_in_obj_sys
) {
	this->object = intersectiong_object;
	intersection_point = intersection_vector;
	this->surfacenormal_in_intersection_point = surfacenormal_in_intersection_point;
	distance_of_ray_in_m = distance_of_ray_support_to_intersection;
	this->_from_outside_to_inside = 
		ray_is_running_from_outside_to_inside(incident_in_obj_sys);
}
//------------------------------------------------------------------------------
bool Intersection::does_intersect()const {
	return object != SurfaceEntity::void_object;
}
//------------------------------------------------------------------------------
const SurfaceEntity * Intersection::get_object()const {
	return object;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_intersection_vector_in_object_system()const {
	return intersection_point;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_intersection_vector_in_world_system()const {
	return object->
		frame2world()->get_transformed_position(intersection_point);
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_surface_normal_in_object_system()const {
	return surfacenormal_in_intersection_point;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_surface_normal_in_world_system()const {
	return object->frame2world()->
		get_transformed_orientation(surfacenormal_in_intersection_point);	
}
//------------------------------------------------------------------------------
double Intersection::get_intersection_distance()const {
	return distance_of_ray_in_m;
}
//------------------------------------------------------------------------------
std::string Intersection::get_print()const {

	std::stringstream out; 
	if(does_intersect()) {
		out << object->get_name() << " in ";
		out << get_intersection_vector_in_world_system() << " after ";
		out << get_intersection_distance() << "m";
	}else
		out << "no intersection with any object";
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
void Intersection::get_reflection_direction_in_object_system(
	Vector3D* vec
)const {
	surfacenormal_in_intersection_point.mirror(vec);
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_reflection_direction_in_world_system(
	Vector3D incomming_dir_in_world
)const {
	object->world2frame()->transform_orientation(&incomming_dir_in_world);
	surfacenormal_in_intersection_point.mirror(&incomming_dir_in_world);
	object->frame2world()->transform_orientation(&incomming_dir_in_world);
	return incomming_dir_in_world;
}
//------------------------------------------------------------------------------
#include "Core/Ray.h"
//------------------------------------------------------------------------------
double Intersection::get_facing_reflection_propability(const double wavelength)const {
	return _from_outside_to_inside ?
	(*object->get_outer_reflection_())(wavelength):
	(*object->get_inner_reflection_())(wavelength);
}
//------------------------------------------------------------------------------
double Intersection::get_refractive_index_going_to(const double wavelength)const {
	return _from_outside_to_inside ?
	(*object->get_inner_refraction_())(wavelength):
	(*object->get_outer_refraction_())(wavelength);
}
//------------------------------------------------------------------------------
bool Intersection::going_to_default_refractive_index()const {
	return _from_outside_to_inside ?
	object->get_inner_refraction_() == SurfaceEntity::default_refr:
	object->get_outer_refraction_() == SurfaceEntity::default_refr;
}
//------------------------------------------------------------------------------
double Intersection::get_refractive_index_coming_from(const double wavelength)const {
	return _from_outside_to_inside ?
	(*object->get_outer_refraction_())(wavelength):
	(*object->get_inner_refraction_())(wavelength);
}
//------------------------------------------------------------------------------
double Intersection::get_outer_half_way_depth(const double wavelength)const {
	return (*object->get_outer_absorption_())(wavelength);
}
//------------------------------------------------------------------------------
double Intersection::get_inner_half_way_depth(const double wavelength)const {
	return (*object->get_inner_absorption_())(wavelength);
}
//------------------------------------------------------------------------------
bool Intersection::boundary_layer_is_transparent()const {
	return object->boundary_layer_is_transparent();
}
//------------------------------------------------------------------------------
const Color Intersection::get_facing_color()const {
	return _from_outside_to_inside ? 
		*object->get_outer_color(): 
		*object->get_inner_color();
}
//------------------------------------------------------------------------------
bool Intersection::ray_is_running_from_outside_to_inside(
	const Vector3D incident_in_obj_sys
)const {
	double projection_of_incident_onto_normal =
		get_surface_normal_in_object_system()*incident_in_obj_sys;

	return projection_of_incident_onto_normal < 0.0;
}
//------------------------------------------------------------------------------
bool Intersection::from_outside_to_inside()const {
	return _from_outside_to_inside;
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_normal_in_faceing_surface_system()const {
	return _from_outside_to_inside ?
	surfacenormal_in_intersection_point:
	surfacenormal_in_intersection_point*-1.0;
}
//------------------------------------------------------------------------------
const HomoTrafo3D* Intersection::world2object()const{
	return object->world2frame();
}
//------------------------------------------------------------------------------
const HomoTrafo3D* Intersection::object2world()const{
	return object->frame2world();
}
//------------------------------------------------------------------------------
bool Intersection::operator<(const Intersection& other)const {
	return this->get_intersection_distance() < other.get_intersection_distance();
}
//------------------------------------------------------------------------------
bool Intersection::compare(const Intersection* one, const Intersection* two){
	return 	one->get_intersection_distance() < two->get_intersection_distance();
}