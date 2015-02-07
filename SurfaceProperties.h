//=================================
// include guard
#ifndef __SurfaceProperties_H_INCLUDED__
#define __SurfaceProperties_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "ColourProperties.h"
#include "ReflectionProperties.h"
//#include "BidirectionalReflectanceDistributionFunction.h"

class SurfaceProperties {
	
	const ColourProperties* color = nullptr;
	const ReflectionProperties* reflection = nullptr;
	//const BidirectionalReflectanceDistributionFunction* brdf = nullptr;
public:

	void set_reflection(const ReflectionProperties* reflection);
	void set_color(const ColourProperties* color);
	//void set_BRDF(const BidirectionalReflectanceDistributionFunction* brdf);

	bool has_reflection()const;
	bool has_color()const;
	//bool has_BRDF()const;

	const ColourProperties* get_color()const;
	const ReflectionProperties* get_reflection()const;
	//const BidirectionalReflectanceDistributionFunction* get_BDRF()const;

	std::string get_print()const;
};
#endif // __SurfaceProperties_H_INCLUDED__ 
