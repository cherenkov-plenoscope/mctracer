//======================================================================
// include guard
#ifndef __COLOURPROPERTIES_H_INCLUDED__
#define __COLOURPROPERTIES_H_INCLUDED__

//======================================================================
// forward declared dependencies

//======================================================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <math.h> 
#include "ReflectionProperties.h"

//======================================================================
class ColourProperties {
private:
	double red_0to255;
	double green_0to255;
	double blue_0to255;
public:
//======================================================================
ColourProperties();
//======================================================================
void set_colour_0to255
(const int new_red,const int new_green,const int new_blue);
//======================================================================
void set_colour_0to255
(const double new_red,const double new_green,const double new_blue);
//======================================================================
unsigned char get_red()const;
unsigned char get_green()const;
unsigned char get_blue()const;
//======================================================================
std::string get_string()const;
//======================================================================
void disp()const;
//======================================================================
void reflection_mix
(const ColourProperties *c ,const ReflectionProperties *refl);
//======================================================================
void mixture(
const ColourProperties *coulour_to_mix_with,
const double mixture_coefficient);
//======================================================================
friend 
std::ostream& operator<<(std::ostream& os, const ColourProperties& col);
};
#endif // __COLOURPROPERTIES_H_INCLUDED__ 
