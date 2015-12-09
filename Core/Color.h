//======================================================================
// include guard
#ifndef __Color_H_INCLUDED__
#define __Color_H_INCLUDED__

//======================================================================
// forward declared dependencies

//======================================================================
// included dependencies
#include <iostream>
#include <string>
#include "Core/Printable.h"

class Color : public Printable{

	unsigned char c_red;
	unsigned char c_green;
	unsigned char c_blue; 
public:
	
	static const Color white;
	static const Color sky_blue;
	static const Color gray;
	static const Color dark_gray;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color grass_green;

	Color();
	Color(const int r, const int g, const int b);
	unsigned char get_red()const;
	unsigned char get_green()const;
	unsigned char get_blue()const;
	std::string get_print()const;
	void reflection_mix(const Color &c,	const double refl);
	void mixture(const Color &cmix, const double mixcoeff);
private:

	void assert_is_in_valid_8Bit_range(const int channel)const;
};
#endif // __Color_H_INCLUDED__ 
