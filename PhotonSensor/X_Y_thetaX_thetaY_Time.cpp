#include "PhotonSensor.h"

namespace PhotonSensor {
	//--------------------------------------------------------------------------
	void X_Y_thetaX_thetaY_Time::reset() {
		arrival_table.clear();
	}
	//--------------------------------------------------------------------------
	void X_Y_thetaX_thetaY_Time::assign_photon_to_this_sensor(const Photon* photon) {
		
		if(	
			photon->get_final_intersection()->get_intersecting_object() == 
			sensor_frame
		) {

			Vector3D final_intersection_incident = 
				photon->get_final_intersection_incident_vector_in_object_frame();


			final_intersection_incident = final_intersection_incident*-1.0;
			double cos_theta_x = final_intersection_incident.x();
			double cos_theta_y = final_intersection_incident.y();

			XYtXtyT hit = {
				photon->get_time_of_flight(),
				photon->get_final_intersection()->
					get_intersection_vector_in_object_system().x(),
				photon->get_final_intersection()->
					get_intersection_vector_in_object_system().y(),
				cos_theta_x,
				cos_theta_y
			};

			arrival_table.push_back(hit);
		}
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> X_Y_thetaX_thetaY_Time::get_arrival_table()const {
		
		std::vector<std::vector<double>> output_table;

		for(XYtXtyT ph : arrival_table) {

			std::vector<double> output_row;

			output_row.push_back(ph.x_intersect);
			output_row.push_back(ph.y_intersect);
			output_row.push_back(ph.theta_x);
			output_row.push_back(ph.theta_y);
			output_row.push_back(ph.arrival_times_since_emission);
	
			output_table.push_back(output_row);
		}

		return output_table;
	}
	//--------------------------------------------------------------------------
} // PhotonSensor