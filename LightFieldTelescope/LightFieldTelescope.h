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
#include "Geometry/SegmentedReflector/SegmentedReflector.h"
#include "PhotonSensor/PhotonSensor.h"
//

using Function::Func1D;

namespace LightFieldTelescope {

	struct Config {

		SegmentedReflector::Config reflector;

		double max_FoV_diameter;
		double pixel_FoV_hex_flat2flat;	
		double housing_overhead;
		double sub_pixel_per_pixel;

		const Func1D* lens_refraction; 
		Config();
	};

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

	class Factory {

		Geometry geometry;
		Config cfg;
		std::vector<PhotonSensor::Sensor*> sub_pixel_sensors;
	public:

		Factory(Config);
		Frame* get_lens_with_name_at_pos(const std::string name, const Vector3D pos);
		Frame* get_lens_array();
		void add_telescope_to_frame(Frame *frame);
		Frame* get_image_sensor_faceplate();
		Frame* get_sub_pixel_sensor_plane();
		Frame* get_sub_pixel_with_name_pos(const std::string name, const Vector3D pos);
		Frame* get_image_sensor_housing()const;
		std::vector<PhotonSensor::Sensor*> get_sensors()const;
	};


	static const Function::Constant pmma_refraction(1.49, Function::Limits(200e-9, 1200e-9));
}
#endif // __PlenopticIactConfig_H_INCLUDED__ 
