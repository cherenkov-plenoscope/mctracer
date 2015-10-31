#include "PhotonSensor.h"

namespace PhotonSensor {
	//--------------------------------------------------------------------------
	void ArrivalTimeSinceEmission::reset() {
		id_time_table.clear();
	}
	//--------------------------------------------------------------------------
	void ArrivalTimeSinceEmission::assign_photon_to_this_sensor(const Photon* photon) {
		
		if(	
			photon->get_final_intersection()->get_intersecting_object() == 
			sensor_frame
		) {

			IdTime idtime = {
				photon->get_id(),
				photon->get_time_of_flight()
			};

			id_time_table.push_back(idtime);
		}
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> ArrivalTimeSinceEmission::get_arrival_table()const {
		
		std::vector<std::vector<double>> output_table;

		for(IdTime idtime : id_time_table) {

			std::vector<double> output_row;
			output_row.push_back(idtime.id);
			output_row.push_back(idtime.arrival_time_since_emission);
			output_table.push_back(output_row);
		}

		return output_table;
	}
	//--------------------------------------------------------------------------
} // PhotonSensor