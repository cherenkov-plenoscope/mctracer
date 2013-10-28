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
//=================================
// the actual class
class SurfaceEntity :public CartesianFrame {
protected:
	bool flag_sensor;
	ReflectionProperties 	reflection;
	ColourProperties 		colour;
public:
//======================
SurfaceEntity();
//======================
bool get_sensor_flag()const;
//======================
void set_surface_properties(
ReflectionProperties *new_reflection_properties, 
ColourProperties *new_colour_properties);
//======================
void disp()const;
//======================
std::string get_surface_propertie_prompt()const;
//======================
bool get_hit_reflection_flag()const;
//======================
ColourProperties get_hit_colour()const;
//======================
const ReflectionProperties* get_ptr2_reflection()const;
//======================
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
