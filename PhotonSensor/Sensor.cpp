#include "PhotonSensor.h"

namespace PhotonSensor {
	//--------------------------------------------------------------------------
	Sensor::Sensor(uint _id, Frame* _sensor_frame) {
		id = _id;
		sensor_frame = _sensor_frame;
	}
	//--------------------------------------------------------------------------
	Sensor::~Sensor() {}
	//--------------------------------------------------------------------------
	uint Sensor::get_id() {
		return id;
	}
	//--------------------------------------------------------------------------
	const Frame* Sensor::get_frame()const {
		return sensor_frame;
	} 
	//--------------------------------------------------------------------------
	void Sensor::assign_photon_to_this_sensor(const Photon* photon) {

	}
	//--------------------------------------------------------------------------
	void Sensor::reset() {

	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> Sensor::get_arrival_table()const {
		std::vector<std::vector<double>> empty;
		return empty;
	}
	//--------------------------------------------------------------------------
	std::string Sensor::get_arrival_table_header()const {
		return "";
	}
	//--------------------------------------------------------------------------
} // PhotonSensor