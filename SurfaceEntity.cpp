#include "SurfaceEntity.h"
//------------------------------------------------------------------------------
const SurfaceEntity* SurfaceEntity::void_object = new SurfaceEntity();
const SurfaceEntity* SurfaceEntity::source_object = new SurfaceEntity();
//------------------------------------------------------------------------------
const ReflectionProperties* SurfaceEntity::default_reflec = new ReflectionProperties(0.0);
const RefractiveIndex* SurfaceEntity::default_refrac = new RefractiveIndex(1.0);
const AbsorptionProperties* SurfaceEntity::default_absorp = new AbsorptionProperties(0.0);
const Color* SurfaceEntity::default_color = new Color(25,25,25);
//------------------------------------------------------------------------------
SurfaceEntity::SurfaceEntity() {

	outer_reflec = default_reflec;
	inner_reflec = default_reflec;

	outer_refrac = default_refrac;
	inner_refrac = default_refrac;

	outer_color = default_color;
	inner_color = default_color;

	outer_absorption = default_absorp;
	inner_absorption = default_absorp;

	_boundary_layer_is_transparent = false;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_allowed_frames_to_propagate_to(
	const Frame* allowed_frame_to_propagate_to
) {	
	this->allowed_frame_to_propagate_to = allowed_frame_to_propagate_to;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_restrictions_on_frames_to_propagate_to()const {
	return allowed_frame_to_propagate_to != nullptr;
}
//------------------------------------------------------------------------------
const Frame* SurfaceEntity::get_allowed_frame_to_propagate_to()const {
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
void SurfaceEntity::set_outer_color(const Color* color) {outer_color = color;}
void SurfaceEntity::set_inner_color(const Color* color) {inner_color = color;}
void SurfaceEntity::set_outer_reflection(const ReflectionProperties* refl) {outer_reflec = refl;}
void SurfaceEntity::set_inner_reflection(const ReflectionProperties* refl) {inner_reflec = refl;}
void SurfaceEntity::set_outer_refraction(const RefractiveIndex* refrac) {
	outer_refrac = refrac;
	if(outer_refrac != default_refrac)
		_boundary_layer_is_transparent = true;
}
void SurfaceEntity::set_inner_refraction(const RefractiveIndex* refrac) {
	inner_refrac = refrac;
	if(inner_refrac != default_refrac)
		_boundary_layer_is_transparent = true;
}
void SurfaceEntity::set_outer_absorption(const AbsorptionProperties* absorp) {outer_absorption = absorp;}
void SurfaceEntity::set_inner_absorption(const AbsorptionProperties* absorp) {inner_absorption = absorp;}
//------------------------------------------------------------------------------
const Color* SurfaceEntity::get_outer_color()const {return outer_color;}
const Color* SurfaceEntity::get_inner_color()const {return inner_color;}
const ReflectionProperties* SurfaceEntity::get_outer_reflection()const {return outer_reflec;}
const ReflectionProperties* SurfaceEntity::get_inner_reflection()const {return inner_reflec;}
const RefractiveIndex* SurfaceEntity::get_outer_refraction()const {return outer_refrac;}
const RefractiveIndex* SurfaceEntity::get_inner_refraction()const {return inner_refrac;}
const AbsorptionProperties* SurfaceEntity::get_outer_absorption()const {return outer_absorption;}
const AbsorptionProperties* SurfaceEntity::get_inner_absorption()const {return inner_absorption;}
bool SurfaceEntity::boundary_layer_is_transparent()const {return _boundary_layer_is_transparent;}
//------------------------------------------------------------------------------
void SurfaceEntity::take_boundary_layer_properties_from(
	const SurfaceEntity* proto
) {
	set_outer_color(proto->get_outer_color());
	set_inner_color(proto->get_inner_color());
	set_outer_reflection(proto->get_outer_reflection());
	set_inner_reflection(proto->get_inner_reflection());
	set_outer_refraction(proto->get_outer_refraction());
	set_inner_refraction(proto->get_inner_refraction());
	set_outer_absorption(proto->get_outer_absorption());
	set_inner_absorption(proto->get_inner_absorption());
}
void SurfaceEntity::take_boundary_layer_properties_but_inside_out_from(
	const SurfaceEntity* proto
) {
	set_outer_color(proto->get_inner_color());
	set_inner_color(proto->get_outer_color());
	set_outer_reflection(proto->get_inner_reflection());
	set_inner_reflection(proto->get_outer_reflection());
	set_outer_refraction(proto->get_inner_refraction());
	set_inner_refraction(proto->get_outer_refraction());
	set_outer_absorption(proto->get_inner_absorption());
	set_inner_absorption(proto->get_outer_absorption());
}
//------------------------------------------------------------------------------
std::string SurfaceEntity::get_surface_print()const {
	std::stringstream out;
	out << " inner surface:\n";
	out << "| color : " << *inner_color << "\n";
	out << "| reflec: " << *inner_reflec << "\n";
	out << "| refrac: " << *inner_refrac << "\n";
	out << "| absorp: " << *inner_absorption << "\n";

	out << " outer surface:\n";
	out << "| color : " << *outer_color << "\n";
	out << "| reflec: " << *outer_reflec << "\n";
	out << "| refrac: " << *outer_refrac << "\n";
	out << "| absorp: " << *outer_absorption << "\n";

	out << " boundary layer: ";
	if(_boundary_layer_is_transparent)
		out << "transparent" << "\n";
	else
		out << "opaque" << "\n";

	if(has_restrictions_on_frames_to_propagate_to())
		out << " inner frame: " << allowed_frame_to_propagate_to->
			get_path_in_tree_of_frames() << "\n";

	return out.str();
}