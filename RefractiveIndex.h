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
#include "Function1D.h"
#include "TracerException.h"

//=======================================================================
class RefractiveIndex {
	
	double simple_refractive_index_independent_of_wavelength = 1.0;
	Function1D* refraction_function = nullptr;
public:
	RefractiveIndex(const double simple_refractive_index_independent_of_wavelength);
	RefractiveIndex(const std::string path_to_file);

	double get_index()const;
	double get_index(double wavelength)const;
private:
	std::string get_print()const;
	void reset_refraction_function();
friend 
	std::ostream& operator<<(std::ostream& os, const RefractiveIndex& refraci_ndex);
};
#endif // __RefractiveIndex_H_INCLUDED__ 