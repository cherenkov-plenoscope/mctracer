//=================================
// include guard
#ifndef __SURFACEENTITY_H_INCLUDED__
#define __SURFACEENTITY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CartesianFrame.h"
#include "ReflectionProperties.h"
#include "ColourProperties.h"

class SurfaceEntity :public CartesianFrame {
protected:
	ReflectionProperties 	reflection;
	ColourProperties 		colour;
public:

	SurfaceEntity(){};

	void set_surface_properties(
		ReflectionProperties *new_reflection_properties, 
		ColourProperties *new_colour_properties
	);

	void disp()const;

	std::string get_surface_propertie_prompt()const;

	bool get_reflection_flag()const;

	ColourProperties get_colour()const;

	const ReflectionProperties* get_reflection_properties()const;
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
