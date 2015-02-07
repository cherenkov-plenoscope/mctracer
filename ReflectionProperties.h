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
#include <vector>
#include <utility>
#include "Function1D.h"
#include "TracerException.h"

//=======================================================================
class ReflectionProperties {

private:	

	bool  reflection_flag = false;
	double simple_reflection_coefficient = 0.0;

	bool reflection_function_was_set = false;
	Function1D* reflection_function = nullptr;
public:

	ReflectionProperties();
	ReflectionProperties(const double refl_coef);

	void SetReflectionCoefficient(const double new_refl_coeff);
	void SetReflectionCoefficient(const std::string path2xml);

	double ReflectionCoefficient() const;
	double ReflectionCoefficient(double wavelength) const;	

	bool flag()const;

	std::string get_print() const;
private:
	void reset_reflection_function();
	void assert_in_range_0_to_1(const double refl)const;
friend 
std::ostream& operator<<(std::ostream& os, const ReflectionProperties& ref);
};
#endif // __REFLECTIONPROPERTIES_H_INCLUDED__ 
