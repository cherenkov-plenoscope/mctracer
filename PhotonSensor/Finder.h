//=================================
// include guard
#ifndef __PHOTONSENSORS_FINDER_H_INCLUDED__
#define __PHOTONSENSORS_FINDER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"
using std::vector;
//=================================

namespace PhotonSensor {

	class FindSensorByFrame {

		bool photon_is_absorbed_by_known_sensor;
		PhotonSensor::Sensor* closest_sensor;
		uint index;
	public:
		FindSensorByFrame(
			const Frame* final_frame,
			const vector<PhotonSensor::Sensor*> *sensors
		);
		uint get_index()const;
		bool frame_is_in_sensors()const;
		PhotonSensor::Sensor* get_sensor()const;
	};

} // PhotonSensor
//------------------------------------------------------------------------------
#endif // __PHOTONSENSORS_FINDER_H_INCLUDED__ 