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

//=================================
namespace PhotonSensor {

	class Sensor {
	protected:

		uint id;
		const Frame* sensor_frame;
	public:

		Sensor(uint _id, Frame* _sensor_frame);
		uint get_id();
		virtual ~Sensor();
		virtual void assign_photon_to_this_sensor(const Photon* photon);
		virtual void reset();
		virtual std::vector<std::vector<double> > get_arrival_table()const;
		virtual std::string get_arrival_table_header()const;
		const Frame* get_frame()const;

	    struct SensorSensorPointerCompare {
			bool operator()(const Sensor* l, const Sensor* r) {
				return l->sensor_frame < r->sensor_frame;
			}
	    };

	    struct FrameSensorPointerCompare {
			bool operator()(const Frame* f, const Sensor* s) {
				return f < s->sensor_frame;
			}
	    };
	};	
} // PhotonSensor
#endif // __PHOTONSENSOR_SENSOR_H_INCLUDED__ 