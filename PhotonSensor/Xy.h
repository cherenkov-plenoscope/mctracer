//=================================
// include guard
#ifndef __PHOTONSENSOR_XY_H_INCLUDED__
#define __PHOTONSENSOR_XY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/Sensor.h"

//=================================
namespace PhotonSensor {
	
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
		double number_photons()const;
		double x_mean()const;
		double y_mean()const;
		double x_std_dev()const;
		double y_std_dev()const;
	};
} // PhotonSensor
#endif // __PHOTONSENSOR_XY_H_INCLUDED__ 