#include "PhotonSensor.h"

namespace PhotonSensor {

	Sensor::Sensor(uint _id, const Frame* _sensor_frame) {
		id = _id;
		sensor_frame = _sensor_frame;
	}

	uint Sensor::get_id()const {
		return id;
	}

	void Sensor::assign_photon_to_this_sensor(const Photon* photon) {
		
		if(	
			photon->get_final_intersection().get_object() == 
			sensor_frame
		) {

			Vec3 final_intersection_incident = 
				photon->get_final_intersection_incident_vector_in_object_frame();

			final_intersection_incident = final_intersection_incident*-1.0;
			double cos_theta_x = final_intersection_incident.x();
			double cos_theta_y = final_intersection_incident.y();

			photon_info info;

			info.id = photon->get_id();
			info.wavelength = photon->get_wavelength();
			info.arrival_time = photon->get_time_of_flight();
			info.x_intersect = photon->get_final_intersection().
					get_intersection_vector_in_object_system().x();
			info.y_intersect = photon->get_final_intersection().
					get_intersection_vector_in_object_system().y();
			info.theta_x = cos_theta_x;
			info.theta_y = cos_theta_y;
			info.production_height_over_sea_level = 
				photon->get_mc_truth()->get_production_height_over_sea_level();

			arrival_table.push_back(info);
		}
	}

	void Sensor::clear_history() {
		arrival_table.clear();
	}

	const Frame* Sensor::get_frame()const {
		return sensor_frame;
	}

	double Sensor::arrival_time_mean()const {

		double t = 0.0;

		for(photon_info ph: arrival_table)
			t += ph.arrival_time;

		return t/arrival_table.size();
	}

	double Sensor::x_mean()const {

		double xm = 0.0;

		for(photon_info ph: arrival_table)
			xm += ph.x_intersect;

		return xm/arrival_table.size();
	}

	double Sensor::y_mean()const {

		double ym = 0.0;

		for(photon_info ph: arrival_table)
			ym += ph.y_intersect;

		return ym/arrival_table.size();
	}

	double Sensor::x_std_dev()const {

		double xm = x_mean();
		double sx = 0.0;

		for(photon_info ph: arrival_table)
			sx += (ph.x_intersect - xm)*(ph.x_intersect - xm);

		return sqrt(sx/arrival_table.size());
	}

	double Sensor::y_std_dev()const {

		double ym = y_mean();
		double sy = 0.0;

		for(photon_info ph: arrival_table)
			sy += (ph.y_intersect - ym)*(ph.y_intersect - ym);

		return sqrt(sy/arrival_table.size());
	}

	double Sensor::point_spread_std_dev()const {

		return hypot(x_std_dev(), y_std_dev());		
	}

	std::vector<std::vector<double>> Sensor::get_arrival_table()const {

		std::vector<std::vector<double>> output_table;

		for(photon_info ph: arrival_table) {

			std::vector<double> output_row;
			output_row.push_back(ph.x_intersect);
			output_row.push_back(ph.y_intersect);
			output_row.push_back(ph.theta_x);
			output_row.push_back(ph.theta_y);
			output_row.push_back(ph.wavelength);
			output_row.push_back(ph.arrival_time);
			output_row.push_back(ph.id);
			output_row.push_back(ph.production_height_over_sea_level);
			output_table.push_back(output_row);
		}

		return output_table;
	}

	std::string Sensor::get_arrival_table_header()const {
		std::stringstream header;
		header << "x: [m], intersection on principal aperture plane\n";
		header << "y: [m], intersection on principal aperture plane\n";
		header << "cos_x: [1], x component of inverse incident direction\n";
		header << "cos_y: [1], x component of inverse incident direction\n";
		header << "            inverse_incident = (cos_x, cos_y, sqrt(1 - cos_x^2 - cos_y^2))^T\n";
		header << "lambda: [m] wavelength\n";
		header << "t: [s] relative arrival time on principal aperture plane.\n";
		header << "id [1] number of the photon in CORSIKA event\n";
		header << "h: [m] production height above sea level\n";
		header << "x\ty\tcos_x\tcos_y\tlambda\tt\tid\th\n";
		return header.str();
	}
} // PhotonSensor