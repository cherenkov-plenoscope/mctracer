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

		Geometry geometry;
		Config cfg;
		std::vector<PhotonSensor::Sensor*> *sub_pixel_sensors;
	public:

		Factory(Config);
		Frame* get_lens_with_name_at_pos(const std::string name, const Vector3D pos);
		Frame* get_lens_array();
		void add_telescope_to_frame(Frame *frame);
		Frame* get_image_sensor_faceplate();
		Frame* get_sub_pixel_sensor_plane();
		Frame* get_sub_pixel_with_name_pos(const std::string name, const Vector3D pos);
		Frame* get_image_sensor_housing()const;
		std::vector<PhotonSensor::Sensor*>* get_sub_pixels()const;
	};

}
#endif // __LightFieldTelescopeFactory_H_INCLUDED__ 
