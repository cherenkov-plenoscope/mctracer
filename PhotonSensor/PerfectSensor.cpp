#include "PhotonSensor/PerfectSensor.h"

namespace PhotonSensor {
	PerfectSensor::PerfectSensor(uint _id, Frame* _sensor_frame)
	: Sensor(_id, _sensor_frame)
	{	}
	//--------------------------------------------------------------------------
	void PerfectSensor::reset() {
		arrival_table.clear();
	}
	//--------------------------------------------------------------------------
	void PerfectSensor::assign_photon_to_this_sensor(const Photon* photon) {
		
		if(	
			photon->get_final_intersection()->get_object() == 
			sensor_frame
		) {

			Vector3D final_intersection_incident = 
				photon->get_final_intersection_incident_vector_in_object_frame();


			final_intersection_incident = final_intersection_incident*-1.0;
			double cos_theta_x = final_intersection_incident.x();
			double cos_theta_y = final_intersection_incident.y();

			XYtXtyT hit = {
				photon->get_id(),
				photon->get_time_of_flight(),
				photon->get_final_intersection()->
					get_intersection_vector_in_object_system().x(),
				photon->get_final_intersection()->
					get_intersection_vector_in_object_system().y(),
				cos_theta_x,
				cos_theta_y,
				photon->get_mc_truth()->get_production_height_over_sea_level()
			};

			arrival_table.push_back(hit);
		}
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> PerfectSensor::get_arrival_table()const {
		
		std::vector<std::vector<double>> output_table;

		for(XYtXtyT ph : arrival_table) {

			std::vector<double> output_row;
			
			output_row.push_back(ph.x_intersect);
			output_row.push_back(ph.y_intersect);
			output_row.push_back(ph.theta_x);
			output_row.push_back(ph.theta_y);
			output_row.push_back(ph.arrival_times_since_emission);
			output_row.push_back(ph.id);
			output_row.push_back(ph.production_height_over_sea_level);
	
			output_table.push_back(output_row);
		}

		return output_table;
	}
	//--------------------------------------------------------------------------
	std::string PerfectSensor::get_arrival_table_header()const {
		std::stringstream header;
		header << "x: [m], intersection on principal aperture plane\n";
		header << "y: [m], intersection on principal aperture plane\n";
		header << "cos_x: [1], x component of inverse incident direction\n";
		header << "cos_y: [1], x component of inverse incident direction\n";
		header << "            inverse_incident = (cos_x, cos_y, sqrt(1 - cos_x^2 - cos_y^2))^T\n";
		header << "t: [s] relative arrival time on principal aperture plane.\n";
		header << "id [1] number of the photon in CORSIKA event\n";
		header << "h: [m] production height above sea level\n";
		header << "x\ty\tcos_x\tcos_y\tt\tid\th\n";
		return header.str();
	}
} // PhotonSensor