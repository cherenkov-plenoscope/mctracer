#include "PhotonSensor.h"
#include <algorithm>
//------------------------------------------------------------------------------
namespace PhotonSensor {
	namespace Sensors {
	//--------------------------------------------------------------------------
	void reset_all_sesnors(
		std::vector<Sensor*> *sensors
	) {
		for(Sensor* sensor : *sensors) 
			sensor->reset();
	}
	//--------------------------------------------------------------------------
	void assign_photons_to_sensors(
		const std::vector<Photon*> *photon_bunch, 
		std::vector<Sensor*> *sensors
	) {
		for(Photon* photon : *photon_bunch) 
			assign_photon_to_sensors(photon, sensors);
	}
	//--------------------------------------------------------------------------
	void assign_photon_to_sensors(
		const Photon* photon,
		std::vector<Sensor*> *sensors
	) {
		const Frame* final_frame = photon->
			get_final_intersection()->get_intersecting_object();

		std::vector<Sensor*>::const_iterator it =
			get_upper_bound_for_final_frame_in_sensors(final_frame, sensors);

		Sensor* closest_sensor = (*(it-1));

		if(it == sensors->begin())
			return;

		if(it == sensors->end() && closest_sensor->get_frame() != final_frame)
			return;

		if(closest_sensor->get_frame() != final_frame)
			return; 

		closest_sensor->assign_photon_to_this_sensor(photon);
	}
	//--------------------------------------------------------------------------
	void sort_photon_sensors_based_on_frames(
		std::vector<Sensor*> *sensors
	) {
		std::sort(
			sensors->begin(), 
			sensors->end(),
			Sensor::SensorSensorPointerCompare()
		);
	}
	//--------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------
	} // Sensors
} // PhotonSensor
