#include "ColourProperties.h"
//======================================================================
ColourProperties::ColourProperties(){
	// default
	set_colour_0to255(128,128,128);
}
//======================================================================
void ColourProperties::set_colour_0to255
(const int new_red,const int new_green,const int new_blue){
	if(	new_red >= 0 && new_red <= 255 &&
		new_green >= 0 && new_green <= 255 &&
		new_blue >= 0 && new_blue <= 255)
	{
		red_0to255=(double)new_red;
		green_0to255=(double)new_green;
		blue_0to255=(double)new_blue;
	}else{
		red_0to255 		= 128.0;
		green_0to255 	= 128.0;
		blue_0to255 	= 128.0;
		std::stringstream out;
		out.str("");
		out<<"Setting color failed!"<<std::endl;
		out<<"Color (r|g|b) is out of 8bit (0-255) bounds."<<std::endl;
		out<<"("<<new_red<<"|"<<new_green<<"|"<<new_blue<<") ";
		out<<"is not valid."<<std::endl;
		std::cout<<out.str();
	}
}
//======================================================================
void ColourProperties::set_colour_0to255
(const double new_red,const double new_green,const double new_blue){
	set_colour_0to255(
	int(round(new_red)),
	int(round(new_green)),
	int(round(new_blue))
	);
}
//======================================================================
unsigned char ColourProperties::get_red()const {
	return (unsigned char)(red_0to255);
}	
//======================================================================
unsigned char ColourProperties::get_green()const {
	return (unsigned char)(green_0to255);
}
//======================================================================
unsigned char ColourProperties::get_blue()const {
	return (unsigned char)(blue_0to255);
}
//======================================================================
std::string ColourProperties::get_string()const{
	std::stringstream out; out.str("");
	out<<"colour: ";
	out<<"("<<red_0to255<<"|"<<green_0to255<<"|"<<blue_0to255<<") ";
	out<<"[rgb]"<<std::endl;
	return out.str();
}
//======================================================================
void ColourProperties::disp()const{
	std::cout<<get_string();
}
//======================================================================
void ColourProperties::reflection_mix
(const ColourProperties *c ,const ReflectionProperties *refl){
	red_0to255 = 
	(1.0 - refl->get_reflection_coefficient())*red_0to255 + 
	refl->get_reflection_coefficient()*c->red_0to255;
	
	green_0to255 = 
	(1.0 - refl->get_reflection_coefficient())*green_0to255 + 
	refl->get_reflection_coefficient()*c->green_0to255;
	
	blue_0to255 = 
	(1.0 - refl->get_reflection_coefficient())*blue_0to255 + 
	refl->get_reflection_coefficient()*c->blue_0to255;
}
//======================================================================
void ColourProperties::mixture(
const ColourProperties *coulour_to_mix_with,
const double mixture_coefficient){
red_0to255 = 
red_0to255 + mixture_coefficient*coulour_to_mix_with->red_0to255;

green_0to255 = 
green_0to255 + mixture_coefficient*coulour_to_mix_with->green_0to255;

blue_0to255 = 
blue_0to255 + mixture_coefficient*coulour_to_mix_with->blue_0to255;
}
