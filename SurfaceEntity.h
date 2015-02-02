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

//=================================
class SurfaceEntity :public CartesianFrame {

	bool color_was_set = false;
	ColourProperties* color;

	bool reflection_coefficient_was_set = false;
	ReflectionProperties* reflection_coefficient;

	bool refractive_index_going_to_was_set = false;
	RefractiveIndex* refractive_index_going_to;

	bool allowed_frame_to_propagate_to_was_set = false;
	CartesianFrame* allowed_frame_to_propagate_to;
public:

	void set_reflection_coefficient(ReflectionProperties* reflection_coefficient);
	void set_color(ColourProperties* color);
	void set_refractive_index(RefractiveIndex* refractive_index);
	void set_allowed_frames_to_propagate_to(CartesianFrame* frame);

	bool has_color()const;
	bool does_reflect()const;
	bool has_restrictions_on_frames_to_propagate_to()const;
	bool does_refract()const;

	ColourProperties get_color()const;
	const ReflectionProperties* get_reflection_properties()const;
	const RefractiveIndex* get_refractive_index_going_to()const;
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
		ReflectionProperties *new_reflection_properties, 
		ColourProperties *new_colour_properties
	);

	bool get_reflection_flag()const;
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
