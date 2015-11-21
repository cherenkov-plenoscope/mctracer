#include "PhotonSensor.h"

namespace PhotonSensor {
	Xy::Xy(uint _id, Frame* _sensor_frame)
	: Sensor(_id, _sensor_frame)
	{	}
	//--------------------------------------------------------------------------
	void Xy::reset() {
		xy_arrival_table.clear();
	}
	//--------------------------------------------------------------------------
	void Xy::assign_photon_to_this_sensor(const Photon* photon) {
		
		if(	
			photon->get_final_intersection()->get_intersecting_object() == 
			sensor_frame
		) {

			std::vector<double> xy = {
				photon->get_final_intersection()->
					get_intersection_vector_in_object_system().x(),
				photon->get_final_intersection()->
					get_intersection_vector_in_object_system().y(),
			};

			xy_arrival_table.push_back(xy);
		}
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> Xy::get_arrival_table()const {
		return xy_arrival_table;
	}
	//--------------------------------------------------------------------------
	std::string Xy::get_arrival_table_header()const {
		std::stringstream header;
		header << "x: [m]\n";
		header << "y: [m]\n";
		return header.str();
	}
	//--------------------------------------------------------------------------
	double Xy::point_spread_std_dev()const {
		// mean position of photon spread
		double xm=0.0;
		double ym=0.0;

		for(uint i=0; i<xy_arrival_table.size(); i++){
			xm = xm + xy_arrival_table[i][0];
			ym = ym + xy_arrival_table[i][1];
		}

		xm = xm/xy_arrival_table.size();
		ym = ym/xy_arrival_table.size();
		
		// estimate psf sigma in x and y
		double sx=0.0;
		double sy=0.0;

		for(uint i=0; i<xy_arrival_table.size(); i++){
			sx = sx + (xy_arrival_table[i][0] - xm)*(xy_arrival_table[i][0] - xm);
			sy = sy + (xy_arrival_table[i][1] - ym)*(xy_arrival_table[i][1] - ym);
		}

		sx = sx/xy_arrival_table.size();
		sy = sy/xy_arrival_table.size();
		sx=sqrt(sx);
		sy=sqrt(sy);

		return hypot(sx*sx, sy*sy);		
	}
} // PhotonSensor