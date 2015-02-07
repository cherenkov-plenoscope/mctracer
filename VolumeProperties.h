//=================================
// include guard
#ifndef __VolumeProperties_H_INCLUDED__
#define __VolumeProperties_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "RefractiveIndex.h"
//#include "AbsorptionProperties.h"

class VolumeProperties {
	
	const RefractiveIndex* refraction = nullptr;
	//const AbsorptionProperties* absorption = nullptr;
public:

	void set_refraction(const RefractiveIndex* refraction);
	//void set_absorption(const AbsorptionProperties* absorption);

	bool has_refraction()const;
	//bool has_absorption()const;

	const RefractiveIndex* get_refraction()const;
	//const AbsorptionProperties* get_absorption()const;

	std::string get_print()const;
};
#endif // __VolumeProperties_H_INCLUDED__ 
