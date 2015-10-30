//=================================
// include guard
#ifndef __PlenopticIactConfig_H_INCLUDED__
#define __PlenopticIactConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/Tools.h"
#include "Core/SurfaceEntity.h"
#include "SegmentedReflectorConfig.h"

using Function::Func1D;

class PlenopticIactConfig {

	// main reflector
	double focal_length;
	double max_aperture_diameter;
	double mirror_facet_inner_hex_radius;
	double mirror_facet_cell_overhead;
	//Func1D* mirror_facet_reflecivity_vs_wavelength;

	//image sensor
	double field_of_view;
	double field_of_view_per_pixel_flat_to_flat;
	double pixels_sensor_radius_overhead;
	//Func1D* pixel_lens_refractivity_vs_wavelength;

	// depending fields
	std::vector<Vector3D> pixel_grid;
	static constexpr double outer_over_inner_hex_radius = 2.0/sqrt(3.0);
public:	
	PlenopticIactConfig();
	void init_pending_fields();
	// reflector
	double max_aperture_radius()const;
	double min_FNumber()const;
	double naive_aperture_area()const;
	// mirror facets
	//double mirror_facet_inner_hex_radius;
	//double mirror_facet_cell_overhead;	
	// pixel
	double pixels_micro_lens_min_FNumber()const;
	double pixels_micro_lens_focal_length()const;
	double pixel_outer_hex_radius()const;
	double pixel_inner_hex_radius()const;
	double pixels_sensor_radius()const;
	double max_image_sensor_radius()const;
	// image sensor
	double image_sensor_area()const;
	std::string get_print()const;
};
#endif // __PlenopticIactConfig_H_INCLUDED__ 
