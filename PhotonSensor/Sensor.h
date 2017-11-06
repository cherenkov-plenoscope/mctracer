//=================================
// include guard
#ifndef __PHOTONSENSOR_SENSOR_H_INCLUDED__
#define __PHOTONSENSOR_SENSOR_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Photon.h"
#include "ArrivalInformation.h"


//=================================
namespace PhotonSensor {

	class Sensor {
	protected:
		
		const Frame* sensor_frame;
	public:
		std::vector<ArrivalInformation> arrival_table;

		unsigned int id;
		Sensor(unsigned int _id, const Frame* _sensor_frame);
		unsigned int get_id()const;
		void assign_photon_to_this_sensor(const Photon* photon);
		void clear_history();
		const Frame* get_frame()const;

		double x_mean()const;
		double y_mean()const;
		double x_std_dev()const;
		double y_std_dev()const;
		double point_spread_std_dev()const;

		double arrival_time_mean()const;

		std::vector<std::vector<double>> get_arrival_table()const;
		std::string get_arrival_table_header()const;

	    struct FrameSensorByFramePointerCompare {
			bool operator()(const Frame* f, const Sensor* s) {
				return f < s->sensor_frame;
			}
	    };
	};
} // PhotonSensor
#endif // __PHOTONSENSOR_SENSOR_H_INCLUDED__ 