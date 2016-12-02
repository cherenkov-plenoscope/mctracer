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
		
		arrival_table.emplace_back(// ArrivalInformation
			// id
			photon->get_simulation_truth_id(),
			// wavelength
			photon->get_wavelength(),
			// arrival_time
			photon->get_time_of_flight(),
			// x
			photon->get_final_intersection().
				get_intersection_vector_in_object_system().x(),
			// y
			photon->get_final_intersection().
				get_intersection_vector_in_object_system().y(),
			// tx
			-1.0*photon->get_final_intersection_incident_vector_in_object_frame().x(),
			// ty
			-1.0*photon->get_final_intersection_incident_vector_in_object_frame().y()
		);
	}

	void Sensor::clear_history() {
		arrival_table.clear();
	}

	const Frame* Sensor::get_frame()const {
		return sensor_frame;
	}

	double Sensor::arrival_time_mean()const {

		double t = 0.0;

		for(ArrivalInformation ph: arrival_table)
			t += ph.arrival_time;

		return t/arrival_table.size();
	}

	double Sensor::x_mean()const {

		double xm = 0.0;

		for(ArrivalInformation ph: arrival_table)
			xm += ph.x_intersect;

		return xm/arrival_table.size();
	}

	double Sensor::y_mean()const {

		double ym = 0.0;

		for(ArrivalInformation ph: arrival_table)
			ym += ph.y_intersect;

		return ym/arrival_table.size();
	}

	double Sensor::x_std_dev()const {

		double xm = x_mean();
		double sx = 0.0;

		for(ArrivalInformation ph: arrival_table)
			sx += (ph.x_intersect - xm)*(ph.x_intersect - xm);

		return sqrt(sx/arrival_table.size());
	}

	double Sensor::y_std_dev()const {

		double ym = y_mean();
		double sy = 0.0;

		for(ArrivalInformation ph: arrival_table)
			sy += (ph.y_intersect - ym)*(ph.y_intersect - ym);

		return sqrt(sy/arrival_table.size());
	}

	double Sensor::point_spread_std_dev()const {

		return hypot(x_std_dev(), y_std_dev());		
	}

	vector<vector<double>> Sensor::get_arrival_table()const {

		vector<vector<double>> output_table;

		for(ArrivalInformation ph: arrival_table) {

			vector<double> output_row;
			output_row.push_back(ph.x_intersect);
			output_row.push_back(ph.y_intersect);
			output_row.push_back(ph.theta_x);
			output_row.push_back(ph.theta_y);
			output_row.push_back(ph.wavelength);
			output_row.push_back(ph.arrival_time);
			output_row.push_back(ph.simulation_truth_id);
			output_table.push_back(output_row);
		}

		return output_table;
	}

	string Sensor::get_arrival_table_header()const {
		std::stringstream header;
		header << "x: [m], intersection on principal aperture plane\n";
		header << "y: [m], intersection on principal aperture plane\n";
		header << "cos_x: [1], x component of inverse incident direction\n";
		header << "cos_y: [1], x component of inverse incident direction\n";
		header << "            inverse_incident = (cos_x, cos_y, sqrt(1 - cos_x^2 - cos_y^2))^T\n";
		header << "lambda: [m] wavelength\n";
		header << "t: [s] relative arrival time on principal aperture plane.\n";
		header << "id [1] simulation_truth_id\n";
		header << "x\ty\tcos_x\tcos_y\tlambda\tt\tid\n";
		return header.str();
	}
} // PhotonSensor