#include "SurfaceEntity.h"
//======================
void SurfaceEntity::set_surface_properties(
	ReflectionProperties *new_reflection_properties, 
	ColourProperties *new_colour_properties
) {
	reflection = *new_reflection_properties;
	colour = *new_colour_properties;
}
//======================
void SurfaceEntity::disp()const{
	std::stringstream out;

	out << get_frame_string();
	out << get_surface_propertie_prompt();
	out << "__________________________" << std::endl;
	std::cout << out.str();
}
//======================
std::string SurfaceEntity::get_surface_propertie_prompt()const{
	std::stringstream out;
	
	out << "|| surface: \n";
	out << "|| " << reflection.get_string();
	out << "|| " << colour.get_string();
	return out.str();
}
//======================
bool SurfaceEntity::get_reflection_flag()const{
	return reflection.flag();
}
//======================
ColourProperties SurfaceEntity::get_colour()const{
	return colour;
}
//======================
const ReflectionProperties* SurfaceEntity::get_reflection_properties()const{
	return &reflection;
} 
//======================
