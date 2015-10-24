//=================================
// include guard
#ifndef __PLENOPTICHEXPIXELGENERATOR_H_INCLUDED__
#define __PLENOPTICHEXPIXELGENERATOR_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "BiConvexLensHexBound.h"
#include "Tools/Tools.h"

using Function::Func1D;

struct PlenopticIactCnfig{
	// main reflector
	double focal_length;
	double max_aperture_diameter;
	double mirror_facet_inner_hex_radius;
	//Func1D* mirror_facet_reflecivity_vs_wavelength;

	//image sensor
	double field_of_view;
	double field_of_view_per_pixel;
	//Func1D* pixel_lens_refractivity_vs_wavelength;

	PlenopticIactCnfig() {
		// main reflector
		focal_length = 70.0;
		max_aperture_diameter = 50.0;
		mirror_facet_inner_hex_radius = 0.60;
		//mirror_facet_reflecivity_vs_wavelength = &Function::Constant::void_function;

		//image sensor
		double field_of_view = Deg2Rad(6.5);
		double field_of_view_per_pixel = Deg2Rad(0.1);
		//Func1D* pixel_lens_refractivity_vs_wavelength;		
	}
};

//=================================
class PlenopticHexPixelGenerator{

	double pixel_outer_hex_radius;
	double fnumber_of_main_optic;
	double plenoptic_pixel_focal_length;
	double plenoptic_image_sensor_overhead;

public:
	PlenopticHexPixelGenerator(const PlenopticIactCnfig config);
	Frame* get_pixel()const;
private:
	
};
#endif // __PLENOPTICHEXPIXELGENERATOR_H_INCLUDED__ 
