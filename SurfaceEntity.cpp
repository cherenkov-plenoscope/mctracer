#include "SurfaceEntity.h"
//======================
SurfaceEntity::SurfaceEntity(){
}
//======================
void SurfaceEntity::set_surface_properties(
ReflectionProperties *new_reflection_properties, 
ColourProperties *new_colour_properties){
	reflection 	= *new_reflection_properties;
	colour 		= *new_colour_properties;
}
//======================
void SurfaceEntity::disp()const{
	std::stringstream out;
	out.str("");
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<"__________________________"<<std::endl;
	std::cout<<out.str();
}
//======================
std::string SurfaceEntity::get_surface_propertie_prompt()const{
	std::stringstream out;
	out.str("");
	out<<"|| surface: "<<std::endl;
	out<<"|| "<<reflection.get_string();
	out<<"|| "<<colour.get_string();
	return out.str();
}
//======================
bool SurfaceEntity::get_hit_reflection_flag()const{
	return reflection.flag();
}
//======================
ColourProperties SurfaceEntity::get_hit_colour()const{
	return colour;
}
//======================
const ReflectionProperties* SurfaceEntity::get_ptr2_reflection()const{
	return &reflection;
} 
//======================
