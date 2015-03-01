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

class ColourProperties {

	double red_0to255;
	double green_0to255;
	double blue_0to255;
public:

	ColourProperties();
	ColourProperties(const int r, const int g, const int b);
	ColourProperties(const double r, const double g, const double b);

	void set_RGB_0to255(
		const int red,
		const int green,
		const int blue
	);

	void set_RGB_0to255(
		const double red,
		const double green,
		const double blue
	);

	unsigned char get_R_as_uchar()const;
	unsigned char get_G_as_uchar()const;
	unsigned char get_B_as_uchar()const;
	
	double get_R_as_double()const;
	double get_G_as_double()const;
	double get_B_as_double()const;

	std::string get_print()const;

	void reflection_mix(
		const ColourProperties *c,
		const double refl_coeff
	);

	void mixture(
		const ColourProperties *coulour_to_mix_with,
		const double mixture_coefficient
	);

	friend std::ostream& operator<<(std::ostream& os, const ColourProperties& col);
private:
	void set_to_default_color();
	void assert_is_in_valid_8Bit_range(const int channel)const;
	void assert_is_in_valid_8Bit_range(const double channel)const;
};
#endif // __COLOURPROPERTIES_H_INCLUDED__ 
