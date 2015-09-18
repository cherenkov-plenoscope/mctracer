//======================================================================
// include guard
#ifndef __AbsorptionProperties_H_INCLUDED__
#define __AbsorptionProperties_H_INCLUDED__

//======================================================================
// forward declared dependencies

//======================================================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include "Tools/AsciiIo.h"
#include "Func1D.h"
#include "TracerException.h"

//=======================================================================
class AbsorptionProperties {
private:	
	double mean_half_way_depth;
	Func1D* half_way_depth_func = nullptr;
public:

	AbsorptionProperties(const double mean_half_way_depth);
	AbsorptionProperties(const std::string path_to_half_way_depth);

	double get_half_way_depth()const;
	double get_half_way_depth(const double wavelength)const;

	std::string get_print() const;
friend 
std::ostream& operator<<(std::ostream& os, const AbsorptionProperties& absorp);
};
#endif // __AbsorptionProperties_H_INCLUDED__ 
