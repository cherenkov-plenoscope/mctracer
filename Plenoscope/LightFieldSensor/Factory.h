//=================================
// include guard
#ifndef __PlenoscopeLightFieldSensorFactory_H_INCLUDED__
#define __PlenoscopeLightFieldSensorFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"
#include "Plenoscope/LightFieldSensor/Geometry.h"

namespace Plenoscope {
namespace LightFieldSensor {

	class Factory {

		PhotonSensors::Sensors *sub_pixel_sensors;
	public:

		const Geometry* geometry;
		Factory(const Geometry* geo);
		void add_light_field_sensor_to_frame(Frame *frame);
		PhotonSensors::Sensors* get_sub_pixels()const;
		void add_lens_array(Frame* frame);
		void add_light_field_sensor_frontplate(Frame* frame);
		void add_image_sensor_housing(Frame* frame);
		void add_pixel_bin_array(Frame* frame);
		void add_pixel_bin_with_name_at_pos(
			Frame* frame,
			const std::string name, 
			const Vec3 pos
		);
		void add_lixel_sensor_plane(Frame* frame);
	};
}//LightFieldSensor
}//Plenoscope
#endif // __PlenoscopeLightFieldSensorFactory_H_INCLUDED__ 
