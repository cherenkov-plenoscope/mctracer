//=================================
// include guard
#ifndef __LIGHTFIELDTELESCOPEFACTORY_H_INCLUDED__
#define __LIGHTFIELDTELESCOPEFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "XmlIO/XmlFileIo.h"

//------------------------------------------------------------------------------
class LightFieldTelescopeFactory : public XmlFileIo{
	const pugi::xml_node node;

public:
	LightFieldTelescopeFactory(const pugi::xml_node node);

	double get_max_FoV_diameter();
	double get_hex_pixel_FoV_flat2flat();
	double get_housing_overhead();
	double get_sub_pixel_per_pixel();
	std::string get_refraction_vs_wavelength();

};
#endif // __LIGHTFIELDTELESCOPEFACTORY_H_INCLUDED__ 