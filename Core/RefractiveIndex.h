//======================================================================
// include guard
#ifndef __RefractiveIndex_H_INCLUDED__
#define __RefractiveIndex_H_INCLUDED__

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
class RefractiveIndex {
	
	const Function::Func1D* refraction_vs_wavelength;
public:
	RefractiveIndex(const double n);
	RefractiveIndex(const Function::Func1D* n_vs_lambda);

	double get_index()const;
	double get_index(double wavelength)const;
private:
	std::string get_print()const;
friend 
	std::ostream& operator<<(std::ostream& os, const RefractiveIndex& refraci_ndex);
};
#endif // __RefractiveIndex_H_INCLUDED__ 