#include "PhotonSensor.h"

namespace PhotonSensor {
	//--------------------------------------------------------------------------
	void X_Y_Time::reset() {
		arrival_times_since_emission.clear();
		x_intersect.clear();
		y_intersect.clear();
	}
	//--------------------------------------------------------------------------
	void X_Y_Time::assign_photon_to_this_sensor(const Photon* photon) {
		
		if(	
			photon->get_final_intersection()->get_intersecting_object() == 
			sensor_frame
		) {
			arrival_times_since_emission.push_back(photon->get_time_of_flight());

			x_intersect.push_back(photon->get_final_intersection()->
				get_intersection_vector_in_object_system().x());

			y_intersect.push_back(photon->get_final_intersection()->
				get_intersection_vector_in_object_system().y());
		}
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> X_Y_Time::get_arrival_table()const {
		
		std::vector<std::vector<double>> table;

		for(uint row=0; row<arrival_times_since_emission.size(); row++) {

			std::vector<double> arrival_row;

			arrival_row.push_back(x_intersect.at(row));
			arrival_row.push_back(y_intersect.at(row));
			arrival_row.push_back(arrival_times_since_emission.at(row));
		
			table.push_back(arrival_row);
		}

		return table;
	}
	//--------------------------------------------------------------------------
} // PhotonSensor