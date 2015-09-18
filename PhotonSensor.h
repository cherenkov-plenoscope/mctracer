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
	PhotonSensor(uint _ident, Frame* _sensor_frame) {
		ident = _ident;
		sensor_frame = _sensor_frame;
	};
	uint get_id() {return ident;};
	virtual void add(const Photon* photon) {
		if(photon->get_final_intersection()->get_intersecting_object() == sensor_frame) {
			arrival_times_since_emission.push_back(photon->get_time_of_flight());
			x_intersect.push_back(photon->get_final_intersection()->
				get_intersection_vector_in_object_system().x());
			y_intersect.push_back(photon->get_final_intersection()->
				get_intersection_vector_in_object_system().y());
		}
	}
	Frame* get_frame()const {return sensor_frame;}
};
#endif // __PHOTONSENSOR_H_INCLUDED__ 
