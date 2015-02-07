#include "SurfaceEntity.h"
//------------------------------------------------------------------------------
void SurfaceEntity::set_color(const ColourProperties* color){
	color_was_set = true;
	this->color = color;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_reflection_properties(
	const ReflectionProperties* reflection_coefficient
) {
	reflection_coefficient_was_set = true;
	this->reflection_coefficient = reflection_coefficient;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_refraction_properties(
	const RefractiveIndex* refractive_index_going_to
) {
	refractive_index_going_to_was_set = true;
	this->refractive_index_going_to = refractive_index_going_to;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_allowed_frames_to_propagate_to(
	const CartesianFrame* allowed_frame_to_propagate_to
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
const ColourProperties* SurfaceEntity::get_color()const {
	return color;
}
//------------------------------------------------------------------------------
const ReflectionProperties* SurfaceEntity::get_reflection_properties()const {
	return reflection_coefficient;
}
//------------------------------------------------------------------------------
const RefractiveIndex* SurfaceEntity::get_refraction_properties()const {
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
	
	out << "| reflection: ";
	if(does_reflect())
		out << reflection_coefficient;
	else 
		out << "none";
	out << "\n";
	
	out << "| color: ";
	if(has_color())
		out << *color;
	else 
		out << "none";
	out << "\n";
	
	return out.str();
}
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
void SurfaceEntity::set_surface_properties(
	const ReflectionProperties *new_reflection_properties, 
	const ColourProperties *new_colour_properties
) {
	ColourProperties* col;
	col = new ColourProperties;
	*col = *new_colour_properties;
	color = col;


	ReflectionProperties* refl;
	refl = new ReflectionProperties;
	*refl = *new_reflection_properties;
	reflection_coefficient = refl;

	reflection_coefficient_was_set = reflection_coefficient->flag();
	color_was_set = true;
}