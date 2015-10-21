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
#include "Core/Function/Func1DFunction.h"
#include "TracerException.h"

//=======================================================================
class ReflectionProperties {
private:	

	const Function::Func1D* reflectivity_vs_wavelength;
public:
	
	ReflectionProperties(const double refl_coef);
	ReflectionProperties(const Function::Func1D* _reflectivity_vs_wavelength);

	double operator()(const double wavelength)const;
	double operator()()const;

	std::string get_print() const;
private:

	void assert_in_range_0_to_1(const double refl)const;
	friend std::ostream& operator<<(std::ostream& os, const ReflectionProperties& ref);
};
#endif // __REFLECTIONPROPERTIES_H_INCLUDED__ 
