//=================================
// include guard
#ifndef __SURFACEENTITY_H_INCLUDED__
#define __SURFACEENTITY_H_INCLUDED__

//=================================
// forward declared dependencies

class RayForPropagation;
//=================================
// included dependencies
#include "Core/Frame.h"
#include "Color.h"
#include "Core/Function/Function.h"

//=================================
class SurfaceEntity :public Frame {

	const Frame* allowed_frame_to_propagate_to = Frame::void_frame;

	bool _boundary_layer_is_transparent;
	
	const Color* outer_color;
	const Color* inner_color;

	const Function::Func1D* outer_reflection_vs_wavelength;
	const Function::Func1D* inner_reflection_vs_wavelength;

	const Function::Func1D* outer_refraction_vs_wavelength;
	const Function::Func1D* inner_refraction_vs_wavelength;

	const Function::Func1D* outer_absorption_vs_wavelength;
	const Function::Func1D* inner_absorption_vs_wavelength;
public:	

	static const SurfaceEntity* void_object;
	static const SurfaceEntity* source_object;

	SurfaceEntity();
	SurfaceEntity(
		const std::string new_name,
        const Vector3D    new_pos,
        const Rotation3D  new_rot
    );
	void set_outer_color(const Color* color);
	void set_inner_color(const Color* color);
	void set_outer_reflection(const Function::Func1D* refl);
	void set_inner_reflection(const Function::Func1D* refl);	
	void set_outer_refraction(const Function::Func1D* refrac);
	void set_inner_refraction(const Function::Func1D* refrac);
	void set_outer_absorption(const Function::Func1D* absorp);
	void set_inner_absorption(const Function::Func1D* absorp);
	void set_allowed_frames_to_propagate_to(const Frame* frame);
	void take_boundary_layer_properties_from(const SurfaceEntity* proto);
	void take_boundary_layer_properties_but_inside_out_from(
		const SurfaceEntity* proto
	);
	const Color* get_outer_color()const;
	const Color* get_inner_color()const;
	const Function::Func1D* get_outer_reflection_()const;
	const Function::Func1D* get_inner_reflection_()const;
	const Function::Func1D* get_outer_refraction_()const;
	const Function::Func1D* get_inner_refraction_()const;
	const Function::Func1D* get_outer_absorption_()const;
	const Function::Func1D* get_inner_absorption_()const;
	const Frame* get_allowed_frame_to_propagate_to()const;
	bool boundary_layer_is_transparent()const;
	bool has_restrictions_on_frames_to_propagate_to()const;
	std::string get_print()const;

	static const Function::Limits default_wavelength_range;
	static const Function::Func1D* default_refl;
	static const Function::Func1D* default_refr;
	static const Function::Func1D* default_abso;
	static const Color* default_color;
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
