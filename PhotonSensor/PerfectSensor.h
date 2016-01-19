//=================================
// include guard
#ifndef __PHOTONSENSOR_PerfectSensor_H_INCLUDED__
#define __PHOTONSENSOR_PerfectSensor_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/Sensor.h"

//=================================
namespace PhotonSensor {

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
		void clear_history();
		std::vector<std::vector<double> > get_arrival_table()const;
		std::vector<double> get_arrival_times()const;
		std::string get_arrival_table_header()const;
		uint get_number_of_photons()const;
	};
} // PhotonSensor
#endif // __PHOTONSENSOR_PerfectSensor_H_INCLUDED__ 