#include "PhotonSensors.h"
#include <algorithm>
//------------------------------------------------------------------------------
namespace PhotonSensors {
	using PhotonSensor::Sensor;
//------------------------------------------------------------------------------
void reset_all_sesnors(
	std::vector<Sensor*> *sensors
) {
	for(Sensor* sensor : *sensors) 
		sensor->reset();
}
//------------------------------------------------------------------------------
void assign_photons_to_sensors(
	const std::vector<Photon*> *photon_bunch, 
	std::vector<Sensor*> *sensors
) {
	for(Photon* photon : *photon_bunch) 
		assign_photon_to_sensors(photon, sensors);
}
//------------------------------------------------------------------------------
void assign_photon_to_sensors(
	const Photon* photon,
	std::vector<Sensor*> *sensors
) {
	FindSensor sens_finder(photon, sensors);

	if(sens_finder.is_absorbed_by_known_sensor())
		sens_finder.get_sensor()->assign_photon_to_this_sensor(photon);
}
//------------------------------------------------------------------------------
void sort_photon_sensors_based_on_frames(
	std::vector<Sensor*> *sensors
) {
	std::sort(
		sensors->begin(), 
		sensors->end(),
		Sensor::SensorSensorPointerCompare()
	);
}
//------------------------------------------------------------------------------
std::vector<Sensor*>::const_iterator get_upper_bound_for_final_frame_in_sensors(
	const Frame* final_frame,
	const std::vector<Sensor*>* sensors
) {
	return std::upper_bound(
		sensors->begin(),
		sensors->end(), 
		final_frame, 
		Sensor::FrameSensorPointerCompare()
	);	
}
//------------------------------------------------------------------------------
FindSensor::FindSensor(
	const Photon* photon,
	std::vector<Sensor*> *sensors
) {
	const Frame* final_frame = photon->
		get_final_intersection()->get_object();

	std::vector<Sensor*>::const_iterator it =
		get_upper_bound_for_final_frame_in_sensors(final_frame, sensors);

	photon_is_absorbed_by_known_sensor = true;

	if(it == sensors->begin()) {
		photon_is_absorbed_by_known_sensor = false;
		return;
	}

	if(it == sensors->end() && (*(it-1))->get_frame() != final_frame) {
		photon_is_absorbed_by_known_sensor = false;
		return;
	}

	if((*(it-1))->get_frame() != final_frame) {
		photon_is_absorbed_by_known_sensor = false; 
		return;
	}

	closest_sensor = (*(it-1));
}
//------------------------------------------------------------------------------
bool FindSensor::is_absorbed_by_known_sensor()const {
	return photon_is_absorbed_by_known_sensor;
}
//------------------------------------------------------------------------------
Sensor* FindSensor::get_sensor()const {
	return closest_sensor;
}
//------------------------------------------------------------------------------
} // PhotonSensors