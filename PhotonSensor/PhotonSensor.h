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
namespace PhotonSensor {
	//--------------------------------------------------------------------------
	class Sensor {
	protected:

		uint id;
		const Frame* sensor_frame;
	public:

		Sensor(uint _id, Frame* _sensor_frame);
		uint get_id();
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
	//--------------------------------------------------------------------------
	class PerfectSensor : public Sensor{
	private:

		struct XYtXtyT {
			uint id;
			double arrival_times_since_emission;
			double x_intersect;
			double y_intersect;
			double theta_x;
			double theta_y;
			double production_height_over_sea_level;		
		};

		std::vector<XYtXtyT> arrival_table;
	public:
		PerfectSensor(uint _id, Frame* _sensor_frame);
		void assign_photon_to_this_sensor(const Photon* photon);
		void reset();
		std::vector<std::vector<double> > get_arrival_table()const;
		std::string get_arrival_table_header()const;
	};
	//--------------------------------------------------------------------------
	class Xy : public Sensor{
	private:

		std::vector<std::vector<double>> xy_arrival_table;
	public:
		Xy(uint _id, Frame* _sensor_frame);
		void assign_photon_to_this_sensor(const Photon* photon);
		void reset();
		std::vector<std::vector<double> > get_arrival_table()const;
		std::string get_arrival_table_header()const;
		double point_spread_std_dev()const;
	};
	//--------------------------------------------------------------------------
} // PhotonSensor
//------------------------------------------------------------------------------
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
} // PhotonSensors
//------------------------------------------------------------------------------
#endif // __PHOTONSENSOR_H_INCLUDED__ 