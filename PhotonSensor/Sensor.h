//=================================
// include guard
#ifndef __PHOTONSENSOR_SENSOR_H_INCLUDED__
#define __PHOTONSENSOR_SENSOR_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <math.h>
#include <sstream>
#include "Core/Photon.h"
using std::vector;
using std::string;

//=================================
namespace PhotonSensor {

	struct PhotonInfo {
		int simulation_truth_id;
		double wavelength;
		double arrival_time;
		double x_intersect;
		double y_intersect;
		double theta_x;
		double theta_y;		
	};

	class Sensor {
	protected:
		
		const Frame* sensor_frame;
	public:
		vector<PhotonInfo> arrival_table;

		uint id;
		Sensor(uint _id, const Frame* _sensor_frame);
		uint get_id()const;
		void assign_photon_to_this_sensor(const Photon* photon);
		void clear_history();
		const Frame* get_frame()const;

		double x_mean()const;
		double y_mean()const;
		double x_std_dev()const;
		double y_std_dev()const;
		double point_spread_std_dev()const;

		double arrival_time_mean()const;

		vector<vector<double>> get_arrival_table()const;
		string get_arrival_table_header()const;

	    struct FrameSensorByFramePointerCompare {
			bool operator()(const Frame* f, const Sensor* s) {
				return f < s->sensor_frame;
			}
	    };
	};
} // PhotonSensor
#endif // __PHOTONSENSOR_SENSOR_H_INCLUDED__ 