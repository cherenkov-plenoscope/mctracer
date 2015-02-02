#include "SurfaceEntity.h"
//------------------------------------------------------------------------------
void SurfaceEntity::set_color(ColourProperties* color){
	color_was_set = true;
	this->color = color;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_reflection_coefficient(
	ReflectionProperties* reflection_coefficient
) {
	reflection_coefficient_was_set = true;
	this->reflection_coefficient = reflection_coefficient;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_refractive_index(
	RefractiveIndex* refractive_index_going_to
) {
	refractive_index_going_to_was_set = true;
	this->refractive_index_going_to = refractive_index_going_to;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_allowed_frames_to_propagate_to(
	CartesianFrame* allowed_frame_to_propagate_to
) {	
	allowed_frame_to_propagate_to_was_set = false;
	this->allowed_frame_to_propagate_to =  allowed_frame_to_propagate_to;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_color()const {
	return color_was_set;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::does_reflect()const {
	return reflection_coefficient_was_set;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_restrictions_on_frames_to_propagate_to()const {
	return allowed_frame_to_propagate_to_was_set;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::does_refract()const {
 	return refractive_index_going_to_was_set;
}
//------------------------------------------------------------------------------
ColourProperties SurfaceEntity::get_color()const {
	return *color;
}
//------------------------------------------------------------------------------
const ReflectionProperties* SurfaceEntity::get_reflection_properties()const {
	return reflection_coefficient;
}
//------------------------------------------------------------------------------
const RefractiveIndex* SurfaceEntity::get_refractive_index_going_to()const {
	return refractive_index_going_to;
}
//------------------------------------------------------------------------------
const CartesianFrame* SurfaceEntity::get_allowed_frame_to_propagate_to()const {
	return allowed_frame_to_propagate_to;
}
//------------------------------------------------------------------------------
std::string SurfaceEntity::get_print()const {
	std::stringstream out;
	out << get_frame_print();
	out << get_surface_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string SurfaceEntity::get_surface_print()const {
	std::stringstream out;
	out << "surface: \n";
	out << "| reflection: " << reflection_coefficient->get_string() << "\n";
	out << "| color: " << color->get_string() << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
void SurfaceEntity::set_surface_properties(
	ReflectionProperties *new_reflection_properties, 
	ColourProperties *new_colour_properties
) {
	color = new ColourProperties;
	*color = *new_colour_properties;

	reflection_coefficient = new ReflectionProperties;
	*reflection_coefficient = *new_reflection_properties;
	
	//reflection = *new_reflection_properties;
	//colour = *new_colour_properties;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::get_reflection_flag()const {
	return reflection_coefficient->flag();
}