//=================================
// include guard
#ifndef __SURFACEENTITY_H_INCLUDED__
#define __SURFACEENTITY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CartesianFrame.h"
#include "RefractiveIndex.h"
#include "ReflectionProperties.h"
#include "ColourProperties.h"

#include "SurfaceProperties.h"
#include "VolumeProperties.h"
//=================================
class SurfaceEntity :public CartesianFrame {
// new

	const VolumeProperties* outer_medium;
	const VolumeProperties* inner_medium;

	const SurfaceProperties *outer_surface;
	const SurfaceProperties *inner_surface;
/*
	void set_outer_surface(const SurfaceProperties *outer_surface);
	void set_inner_surface(const SurfaceProperties *inner_surface);
	bool has_inner_surface()const;
	bool has_outer_surface()const;
	const SurfaceProperties *get_outer_surface()const;
	const SurfaceProperties *get_inner_surface()const;
*/
//------------------------------------------------------------------------------
// old
//------------------------------------------------------------------------------
	bool color_was_set = false;
	const ColourProperties* color;

	bool reflection_coefficient_was_set = false;
	const ReflectionProperties* reflection_coefficient;

	bool refractive_index_going_to_was_set = false;
	const RefractiveIndex* refractive_index_going_to;

	bool allowed_frame_to_propagate_to_was_set = false;
	const CartesianFrame* allowed_frame_to_propagate_to;
public:

	void set_reflection_properties(const ReflectionProperties* reflection_coefficient);
	void set_color(const ColourProperties* color);
	void set_refraction_properties(const RefractiveIndex* refractive_index);
	void set_allowed_frames_to_propagate_to(const CartesianFrame* frame);

	bool has_color()const;
	bool does_reflect()const;
	bool has_restrictions_on_frames_to_propagate_to()const;
	bool does_refract()const;

	const ColourProperties* get_color()const;
	const ReflectionProperties* get_reflection_properties()const;
	const RefractiveIndex* get_refraction_properties()const;
	const CartesianFrame* get_allowed_frame_to_propagate_to()const;

	std::string get_print()const;
protected:

	std::string get_surface_print()const;
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
private:

	//ReflectionProperties reflection;
	//ColourProperties colour;
public:

	void set_surface_properties(
		const ReflectionProperties *new_reflection_properties, 
		const ColourProperties *new_colour_properties
	);
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
