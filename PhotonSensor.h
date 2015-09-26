//=================================
// include guard
#ifndef __PHOTONSENSOR_H_INCLUDED__
#define __PHOTONSENSOR_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>
#include "Core/Photon.h"

//=================================
class PhotonSensor {
private:

	uint ident;
	Frame* sensor_frame;
	std::vector<double> arrival_times_since_emission;
	std::vector<double> x_intersect;
	std::vector<double> y_intersect;
public:

	PhotonSensor(uint _ident, Frame* _sensor_frame);
	uint get_id();
	virtual void assign_photon_to_this_sensor(const Photon* photon);
	const Frame* get_frame()const;
	std::vector<std::vector<double>> get_arrival_table_x_y_t()const;
	void reset();

    struct SensorSensorPointerCompare {
		bool operator()(const PhotonSensor* l, const PhotonSensor* r) {
			return l->sensor_frame < r->sensor_frame;
		}
    };

    struct FrameSensorPointerCompare {
		bool operator()(const Frame* f, const PhotonSensor* s) {
			return f < s->sensor_frame;
		}
    };
};

namespace PhotonSensors {

	void reset_all_sesnors(
		std::vector<PhotonSensor*> *sensors
	);
	
	void assign_photons_to_sensors(
		const std::vector<Photon*> *photon_bunch, 
		std::vector<PhotonSensor*> *sensors
	);

	void assign_photon_to_sensors(
		const Photon* photon,
		std::vector<PhotonSensor*> *sensors
	);

	void sort_photon_sensors_based_on_frames(
		std::vector<PhotonSensor*> *sensors
	);

	std::vector<PhotonSensor*>::const_iterator get_upper_bound_for_final_frame_in_sensors(
		const Frame* final_frame,
		const std::vector<PhotonSensor*>* sensors
	);
}
#endif // __PHOTONSENSOR_H_INCLUDED__ 