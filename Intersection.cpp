#include "Intersection.h"
//------------------------------------------------------------------------------
const SurfaceEntity* Intersection::void_object = new SurfaceEntity();
const SurfaceEntity* Intersection::source_object = new SurfaceEntity();
//------------------------------------------------------------------------------
Intersection::Intersection() {
	intersecting_object = void_object;
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
	this->intersecting_object = intersectiong_object;
	intersection_point = intersection_vector;
	this->surfacenormal_in_intersection_point = surfacenormal_in_intersection_point;
	distance_of_ray_in_m = distance_of_ray_support_to_intersection;
	this->_from_outside_to_inside = 
		ray_is_running_from_outside_to_inside(incident_in_obj_sys);
}
//------------------------------------------------------------------------------
bool Intersection::does_intersect()const {
	return intersecting_object != void_object;
}
//------------------------------------------------------------------------------
const SurfaceEntity * Intersection::get_intersecting_object()const {
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
void Intersection::get_reflection_direction_in_object_system(
	Vector3D* vec
)const {
	surfacenormal_in_intersection_point.mirror(vec);
}
//------------------------------------------------------------------------------
Vector3D Intersection::get_reflection_direction_in_world_system(
	Vector3D incomming_dir_in_world
)const {
	intersecting_object->world2frame()->transform_orientation(&incomming_dir_in_world);
	surfacenormal_in_intersection_point.mirror(&incomming_dir_in_world);
	intersecting_object->frame2world()->transform_orientation(&incomming_dir_in_world);
	return incomming_dir_in_world;
}
//------------------------------------------------------------------------------
#include "Ray.h"
//------------------------------------------------------------------------------
double Intersection::get_facing_reflection_propability()const {
	return _from_outside_to_inside ?
	intersecting_object->get_outer_reflection()->ReflectionCoefficient():
	intersecting_object->get_inner_reflection()->ReflectionCoefficient();
}
double Intersection::get_facing_reflection_propability(const double wavelength)const {
	return _from_outside_to_inside ?
	intersecting_object->get_outer_reflection()->ReflectionCoefficient(wavelength):
	intersecting_object->get_inner_reflection()->ReflectionCoefficient(wavelength);
}
//------------------------------------------------------------------------------
double Intersection::get_refractive_index_going_to()const {
	return _from_outside_to_inside ?
	intersecting_object->get_inner_refraction()->get_index():
	intersecting_object->get_outer_refraction()->get_index();
}
double Intersection::get_refractive_index_going_to(const double wavelength)const {
	return _from_outside_to_inside ?
	intersecting_object->get_inner_refraction()->get_index(wavelength):
	intersecting_object->get_outer_refraction()->get_index(wavelength);
}
double Intersection::get_refractive_index_coming_from()const {
	return _from_outside_to_inside ?
	intersecting_object->get_outer_refraction()->get_index():
	intersecting_object->get_inner_refraction()->get_index();
}
double Intersection::get_refractive_index_coming_from(const double wavelength)const {
	return _from_outside_to_inside ?
	intersecting_object->get_outer_refraction()->get_index(wavelength):
	intersecting_object->get_inner_refraction()->get_index(wavelength);
}
//------------------------------------------------------------------------------
double Intersection::get_outer_half_way_depth()const {
	return intersecting_object->get_outer_absorption()->get_half_way_depth();
}
double Intersection::get_outer_half_way_depth(const double wavelength)const {
	return intersecting_object->get_outer_absorption()->get_half_way_depth(wavelength);
}
double Intersection::get_inner_half_way_depth()const {
	return intersecting_object->get_inner_absorption()->get_half_way_depth();
}
double Intersection::get_inner_half_way_depth(const double wavelength)const {
	return intersecting_object->get_inner_absorption()->get_half_way_depth(wavelength);
}
//------------------------------------------------------------------------------
bool Intersection::boundary_layer_is_transparent()const {
	return intersecting_object->boundary_layer_is_transparent();
}
//------------------------------------------------------------------------------
const ColourProperties Intersection::get_facing_color()const {
	return _from_outside_to_inside ? 
		*intersecting_object->get_outer_color(): 
		*intersecting_object->get_inner_color();
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
	return intersecting_object->world2frame();
}
const HomoTrafo3D* Intersection::object2world()const{
	return intersecting_object->frame2world();
}
//------------------------------------------------------------------------------