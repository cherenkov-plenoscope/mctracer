//=================================
// include guard
#ifndef __PHOTONSENSORS_FINDER_H_INCLUDED__
#define __PHOTONSENSORS_FINDER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"
#include <vector>

//=================================

namespace PhotonSensor {

	class FindSensorByFrame {

		bool photon_is_absorbed_by_known_sensor;
		PhotonSensor::Sensor* closest_sensor;
	public:
		FindSensorByFrame(
			const Frame* final_frame,
			const std::vector<PhotonSensor::Sensor*> *sensors
		);
		bool frame_is_in_sensors()const;
		PhotonSensor::Sensor* get_sensor()const;
	};

} // PhotonSensor
//------------------------------------------------------------------------------
#endif // __PHOTONSENSORS_FINDER_H_INCLUDED__ 