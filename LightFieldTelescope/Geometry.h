//=================================
// include guard
#ifndef __LightFieldTelescopeGeometry_H_INCLUDED__
#define __LightFieldTelescopeGeometry_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/Config.h"

namespace LightFieldTelescope {

	class Geometry {

			const Config cfg;
		
			std::vector<Vector3D> pixel_grid;
			std::vector<Vector3D> sub_pixel_grid;

			double pixel_lens_curv_radius;
			double pixel_lens_mean_refrac;
		public:
			const SegmentedReflector::Geometry reflector;

			Geometry(const Config ncfg);
			std::string get_print()const;
			std::string get_image_sensor_print()const;
			double max_outer_sensor_radius()const;
			double max_FoV_radius()const;
			double pixel_spacing()const;
			double pixel_lens_inner_aperture_radius()const;
			double pixel_lens_outer_aperture_radius()const;
			double pixel_lens_focal_length()const;
			double pixel_lens_f_over_D()const;
			double pixel_lens_curvature_radius()const;
			double pixel_lens_mean_refraction()const;
			double pixel_FoV_hex_flat2flat()const;
			uint number_of_pixels()const;
			double outer_sensor_housing_radius()const;
			std::vector<Vector3D> pixel_positions()const;
			double sub_pixel_per_pixel()const;
			double sub_pixel_sensor_radius()const;
			double sub_pixel_outer_radius()const;
			double sub_pixel_inner_radius()const;
			double sub_pixel_spacing()const;
			std::vector<Vector3D> sub_pixel_positions()const;
		};
}
#endif // __LightFieldTelescopeGeometry_H_INCLUDED__ 
