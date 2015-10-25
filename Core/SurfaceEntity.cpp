#include "SurfaceEntity.h"
#include "Core/Function/LimitsFunction.h"
#include "Core/Function/ConstantFunction.h"
using Function::Func1D;
using Function::Constant;
using Function::Limits;
//------------------------------------------------------------------------------
const SurfaceEntity* SurfaceEntity::void_object = new SurfaceEntity();

const SurfaceEntity* SurfaceEntity::source_object = new SurfaceEntity();

const Color* SurfaceEntity::default_color = &Color::dark_gray;

const Limits SurfaceEntity::default_wavelength_range = 
	Limits(200e-9, 1200e-9);

const Func1D* SurfaceEntity::default_refl = 
	new Constant(0.0, default_wavelength_range);

const Func1D* SurfaceEntity::default_refr = 
	new Constant(1.0, default_wavelength_range);

const Func1D* SurfaceEntity::default_abso = 
	new Constant(0.0, default_wavelength_range);
//------------------------------------------------------------------------------
SurfaceEntity::SurfaceEntity() {
	
	_boundary_layer_is_transparent = false;

	outer_color = default_color;
	inner_color = default_color;

	outer_reflection_vs_wavelength = default_refl;
	inner_reflection_vs_wavelength = default_refl;

	outer_refraction_vs_wavelength = default_refr;
	inner_refraction_vs_wavelength = default_refr;

	outer_absorption_vs_wavelength = default_abso;
	inner_absorption_vs_wavelength = default_abso;
}
//------------------------------------------------------------------------------
const Func1D* SurfaceEntity::get_outer_reflection_()const {
	return outer_reflection_vs_wavelength;
}
//------------------------------------------------------------------------------
const Func1D* SurfaceEntity::get_inner_reflection_()const {
	return inner_reflection_vs_wavelength;
}
//------------------------------------------------------------------------------
const Func1D* SurfaceEntity::get_outer_refraction_()const {
	return outer_refraction_vs_wavelength;
}
//------------------------------------------------------------------------------
const Func1D* SurfaceEntity::get_inner_refraction_()const {
	return inner_refraction_vs_wavelength;
}
//------------------------------------------------------------------------------
const Func1D* SurfaceEntity::get_outer_absorption_()const {
	return outer_absorption_vs_wavelength;
}
//------------------------------------------------------------------------------
const Func1D* SurfaceEntity::get_inner_absorption_()const {
	return inner_absorption_vs_wavelength;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_outer_reflection(const Func1D* reflec) {
	outer_reflection_vs_wavelength = reflec;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_inner_reflection(const Func1D* reflec) {
	inner_reflection_vs_wavelength = reflec;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_outer_refraction(const Func1D* refrac) {
	outer_refraction_vs_wavelength = refrac;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_inner_refraction(const Func1D* refrac) {
	inner_refraction_vs_wavelength = refrac;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_outer_absorption(const Func1D* absorp) {
	outer_absorption_vs_wavelength = absorp;
	if(outer_refraction_vs_wavelength != default_refr)
		_boundary_layer_is_transparent = true;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_inner_absorption(const Func1D* absorp) {
	inner_absorption_vs_wavelength = absorp;
	if(inner_refraction_vs_wavelength != default_refr)
		_boundary_layer_is_transparent = true;	
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_allowed_frames_to_propagate_to(
	const Frame* allowed_frame_to_propagate_to
) {	
	this->allowed_frame_to_propagate_to = allowed_frame_to_propagate_to;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_restrictions_on_frames_to_propagate_to()const {
	return allowed_frame_to_propagate_to != Frame::void_frame;
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
void SurfaceEntity::set_outer_color(const Color* color) {
	outer_color = color;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_inner_color(const Color* color) {
	inner_color = color;
}
//------------------------------------------------------------------------------
const Color* SurfaceEntity::get_outer_color()const {
	return outer_color;
}
//------------------------------------------------------------------------------
const Color* SurfaceEntity::get_inner_color()const {
	return inner_color;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::boundary_layer_is_transparent()const {
	return _boundary_layer_is_transparent;
}
//------------------------------------------------------------------------------
void SurfaceEntity::take_boundary_layer_properties_from(
	const SurfaceEntity* proto
) {
	set_outer_color(proto->get_outer_color());
	set_inner_color(proto->get_inner_color());
	set_outer_reflection(proto->get_outer_reflection_());
	set_inner_reflection(proto->get_inner_reflection_());
	set_outer_refraction(proto->get_outer_refraction_());
	set_inner_refraction(proto->get_inner_refraction_());
	set_outer_absorption(proto->get_outer_absorption_());
	set_inner_absorption(proto->get_inner_absorption_());
}
//------------------------------------------------------------------------------
void SurfaceEntity::take_boundary_layer_properties_but_inside_out_from(
	const SurfaceEntity* proto
) {
	set_outer_color(proto->get_inner_color());
	set_inner_color(proto->get_outer_color());
	set_outer_reflection(proto->get_inner_reflection_());
	set_inner_reflection(proto->get_outer_reflection_());
	set_outer_refraction(proto->get_inner_refraction_());
	set_inner_refraction(proto->get_outer_refraction_());
	set_outer_absorption(proto->get_inner_absorption_());
	set_inner_absorption(proto->get_outer_absorption_());
}
//------------------------------------------------------------------------------
std::string SurfaceEntity::get_surface_print()const {
	
	std::stringstream out;
	out << " inner surface:\n";
	out << "| color : " << *inner_color << "\n";
	out << "| reflec: " << inner_reflection_vs_wavelength->get_print() << "\n";
	out << "| refrac: " << inner_refraction_vs_wavelength->get_print() << "\n";
	out << "| absorp: " << inner_absorption_vs_wavelength->get_print() << "\n";

	out << " outer surface:\n";
	out << "| color : " << *outer_color << "\n";
	out << "| reflec: " << outer_reflection_vs_wavelength->get_print() << "\n";
	out << "| refrac: " << outer_refraction_vs_wavelength->get_print() << "\n";
	out << "| absorp: " << inner_absorption_vs_wavelength->get_print() << "\n";

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
//------------------------------------------------------------------------------