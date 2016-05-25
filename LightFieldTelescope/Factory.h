//=================================
// include guard
#ifndef __LightFieldTelescopeFactory_H_INCLUDED__
#define __LightFieldTelescopeFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"
#include "LightFieldTelescope/Geometry.h"

namespace LightFieldTelescope {

	class Factory {

		PhotonSensors::Sensors *sub_pixel_sensors;
	public:

		const Geometry *geometry;
		Factory(const Geometry *geo);
		void add_telescope_to_frame(Frame *frame);
		void add_light_field_sensor_to_frame(Frame *frame);
		PhotonSensors::Sensors* get_sub_pixels()const;
		Frame* get_lens_array();
		Frame* get_pixel_bin_array();
		Frame* get_light_field_sensor_frontplate();
		Frame* get_sub_pixel_sensor_plane();
		Frame* get_image_sensor_housing()const;
		
		Frame* get_lens_with_name_at_pos(
			const std::string name,
			const Vec3 pos
		);
		
		Frame* get_sub_pixel_with_name_pos(
			const std::string name, 
			const Vec3 pos
		);
		
		Frame* get_pixel_bin_with_name_at_pos(
			const std::string name, 
			const Vec3 pos
		);
	};
}
#endif // __LightFieldTelescopeFactory_H_INCLUDED__ 
