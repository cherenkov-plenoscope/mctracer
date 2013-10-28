//======================================================================
// include guard
#ifndef __REFLECTIONPROPERTIES_H_INCLUDED__
#define __REFLECTIONPROPERTIES_H_INCLUDED__

//======================================================================
// forward declared dependencies

//======================================================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>

//=======================================================================
class ReflectionProperties {
	// this class stores reflection information of an object
private:	
	bool  reflection_flag;
	double reflection_coefficient;
public:
//======================================================================
	ReflectionProperties();
//======================================================================
	void set_reflection_coefficient(const double new_refl_coeff);
//======================================================================
	double get_reflection_coefficient() const;
//======================================================================
	bool get_reflection_flag() const;
//======================================================================
	std::string get_string() const;
//======================================================================
	void disp() const;
};
#endif // __REFLECTIONPROPERTIES_H_INCLUDED__ 
