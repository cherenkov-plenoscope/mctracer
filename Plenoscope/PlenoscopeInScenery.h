//=================================
// include guard
#ifndef __PlenopticIactConfig_H_INCLUDED__
#define __PlenopticIactConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/LightFieldSensor/LightFieldSensor.h"

namespace Plenoscope {
	struct PlenoscopeInScenery {

		PlenoscopeInScenery(const LightFieldSensor::Config config):
			light_field_sensor_geometry(config)
		{};
		Frame* frame;
		PhotonSensors::Sensors* light_field_channels;
		LightFieldSensor::Geometry light_field_sensor_geometry;
	};
}
#endif // __PlenopticIactConfig_H_INCLUDED__ 
