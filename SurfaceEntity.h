//=================================
// include guard
#ifndef __SURFACEENTITY_H_INCLUDED__
#define __SURFACEENTITY_H_INCLUDED__

//=================================
// forward declared dependencies
class RayForPropagation;
//=================================
// included dependencies
#include "Frame.h"
#include "RefractiveIndex.h"
#include "ReflectionProperties.h"
#include "Color.h"
#include "AbsorptionProperties.h"

//=================================
class SurfaceEntity :public Frame {

	const Frame* allowed_frame_to_propagate_to = nullptr;
public:	
	static const SurfaceEntity* void_object;
	static const SurfaceEntity* source_object;
	bool has_restrictions_on_frames_to_propagate_to()const;
	const Frame* get_allowed_frame_to_propagate_to()const;
	void set_allowed_frames_to_propagate_to(const Frame* frame);
	std::string get_print()const;
protected:
	std::string get_surface_print()const;
////////////////////////////////////////////////////////////////////////////////
private:
	static const ReflectionProperties* default_reflec;
	static const RefractiveIndex* default_refrac;
	static const AbsorptionProperties* default_absorp;
	static const Color* default_color;

	const ReflectionProperties* outer_reflec;
	const ReflectionProperties* inner_reflec;

	const RefractiveIndex* outer_refrac;
	const RefractiveIndex* inner_refrac;

	const AbsorptionProperties* outer_absorption;
	const AbsorptionProperties* inner_absorption;

	const Color* outer_color;
	const Color* inner_color;

	bool _boundary_layer_is_transparent;
public:
	SurfaceEntity();
	void set_outer_color(const Color* color);
	void set_inner_color(const Color* color);
	void set_outer_reflection(const ReflectionProperties* refl);
	void set_inner_reflection(const ReflectionProperties* refl);
	void set_outer_refraction(const RefractiveIndex* refrac);
	void set_inner_refraction(const RefractiveIndex* refrac);
	void set_outer_absorption(const AbsorptionProperties* absorp);
	void set_inner_absorption(const AbsorptionProperties* absorp);

	const Color* get_outer_color()const;
	const Color* get_inner_color()const;
	const ReflectionProperties* get_outer_reflection()const;
	const ReflectionProperties* get_inner_reflection()const;
	const RefractiveIndex* get_outer_refraction()const;
	const RefractiveIndex* get_inner_refraction()const;
	const AbsorptionProperties* get_outer_absorption()const;
	const AbsorptionProperties* get_inner_absorption()const;
	bool boundary_layer_is_transparent()const;

	void take_boundary_layer_properties_from(const SurfaceEntity* proto);
	void take_boundary_layer_properties_but_inside_out_from(const SurfaceEntity* proto);
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
