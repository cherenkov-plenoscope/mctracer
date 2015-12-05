//=================================
// include guard
#ifndef __PHOTONSENSORS_H_INCLUDED__
#define __PHOTONSENSORS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"

//=================================

namespace PhotonSensors {

	void reset_all_sesnors(
		std::vector<PhotonSensor::Sensor*> *sensors
	);
	
	void assign_photons_to_sensors(
		const std::vector<Photon*> *photon_bunch, 
		std::vector<PhotonSensor::Sensor*> *sensors
	);

	void assign_photon_to_sensors(
		const Photon* photon,
		std::vector<PhotonSensor::Sensor*> *sensors
	);

	void sort_photon_sensors_based_on_frames(
		std::vector<PhotonSensor::Sensor*> *sensors
	);

	std::vector<PhotonSensor::Sensor*>::const_iterator 
		get_upper_bound_for_final_frame_in_sensors(
		const Frame* final_frame,
		const std::vector<PhotonSensor::Sensor*>* sensors
	);

	class FindSensor {

		bool photon_is_absorbed_by_known_sensor;
		PhotonSensor::Sensor* closest_sensor;
	public:
		FindSensor(
			const Photon* photon,
			std::vector<PhotonSensor::Sensor*> *sensors
		);
		bool is_absorbed_by_known_sensor()const;
		PhotonSensor::Sensor* get_sensor()const;
	};
} // PhotonSensors
//------------------------------------------------------------------------------
#endif // __PHOTONSENSORS_H_INCLUDED__ 