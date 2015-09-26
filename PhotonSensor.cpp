#include "PhotonSensor.h"
#include <algorithm>
#include <omp.h>
//------------------------------------------------------------------------------
PhotonSensor::PhotonSensor(uint _ident, Frame* _sensor_frame) {
	ident = _ident;
	sensor_frame = _sensor_frame;
}
//------------------------------------------------------------------------------
void PhotonSensor::reset() {
	arrival_times_since_emission.clear();
	x_intersect.clear();
	y_intersect.clear();
}
//------------------------------------------------------------------------------
uint PhotonSensor::get_id() {
	return ident;
}
//------------------------------------------------------------------------------
void PhotonSensor::assign_photon_to_this_sensor(const Photon* photon) {
	
	if(	
		photon->get_final_intersection()->get_intersecting_object() == 
		sensor_frame
	) {
		arrival_times_since_emission.push_back(photon->get_time_of_flight());

		x_intersect.push_back(photon->get_final_intersection()->
			get_intersection_vector_in_object_system().x());

		y_intersect.push_back(photon->get_final_intersection()->
			get_intersection_vector_in_object_system().y());
	}
}
//------------------------------------------------------------------------------
std::vector<std::vector<double>> PhotonSensor::get_arrival_table_x_y_t()const {
	std::vector<std::vector<double>> table;

	for(uint row=0; row<arrival_times_since_emission.size(); row++) {

		std::vector<double> arrival_row;

		arrival_row.push_back(x_intersect.at(row));
		arrival_row.push_back(y_intersect.at(row));
		arrival_row.push_back(arrival_times_since_emission.at(row));
	
		table.push_back(arrival_row);
	}

	return table;
}
//------------------------------------------------------------------------------
const Frame* PhotonSensor::get_frame()const {
	return sensor_frame;
} 
//------------------------------------------------------------------------------
namespace PhotonSensors {
	void reset_all_sesnors(
		std::vector<PhotonSensor*> *sensors
	) {
		for(PhotonSensor* sensor : *sensors) 
			sensor->reset();
	}
	//--------------------------------------------------------------------------
	void assign_photons_to_sensors(
		const std::vector<Photon*> *photon_bunch, 
		std::vector<PhotonSensor*> *sensors
	) {
		for(Photon* photon : *photon_bunch) 
			assign_photon_to_sensors(photon, sensors);
	}
	//--------------------------------------------------------------------------
	void assign_photon_to_sensors(
		const Photon* photon,
		std::vector<PhotonSensor*> *sensors
	) {
		const Frame* final_frame = photon->
			get_final_intersection()->get_intersecting_object();

		std::vector<PhotonSensor*>::const_iterator it =
			get_upper_bound_for_final_frame_in_sensors(final_frame, sensors);

		PhotonSensor* closest_sensor = (*(it-1));

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
		std::vector<PhotonSensor*> *sensors
	) {
		std::sort(
			sensors->begin(), 
			sensors->end(),
			PhotonSensor::SensorSensorPointerCompare()
		);
	}
	//--------------------------------------------------------------------------
	std::vector<PhotonSensor*>::const_iterator get_upper_bound_for_final_frame_in_sensors(
		const Frame* final_frame,
		const std::vector<PhotonSensor*>* sensors
	) {
		return std::upper_bound(
			sensors->begin(),
			sensors->end(), 
			final_frame, 
			PhotonSensor::FrameSensorPointerCompare()
		);	
	}
	//--------------------------------------------------------------------------
}
