#include "LightFieldTelescope/Calibration.h"
#include "LightFieldTelescope/CalibIo.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Calibration::Calibration(const Config cfg): 
	telescope_config(cfg), 
	telescope_geometry(cfg) 
{
	number_of_photons = telescope_geometry.total_number_of_sub_pixels()*1e3;
	number_of_photons_per_block = 1e6;
	number_of_blocks = ceil(
		double(number_of_photons)/double(number_of_photons_per_block)
	);

	set_up_photon_properties();
	set_up_principal_aperture_range();
	set_up_field_of_view_range();
	set_up_telescope();
	set_up_telescope_environment();

	run_calibration();
}
//------------------------------------------------------------------------------
void Calibration::set_up_photon_properties() {

	distance_to_travel_before_intersecting_principal_aperture = 1e4;
	callibration_photon_wavelenght = 433e-9;
}
//------------------------------------------------------------------------------
void Calibration::set_up_principal_aperture_range() {

	max_principal_aperture_radius_to_trow_photons_on = 1.05*(
		telescope_geometry.reflector.max_outer_aperture_radius() + 
		telescope_geometry.reflector.thickness_of_dish()*tan(
			telescope_geometry.max_FoV_radius()
		)
	);
}
//------------------------------------------------------------------------------
void Calibration::set_up_field_of_view_range() {

	max_tilt_vs_optical_axis_to_throw_photons_in = 
		telescope_geometry.max_FoV_radius()*1.05;
}
//------------------------------------------------------------------------------
void Calibration::set_up_telescope() {

	telescope = new Frame("telescope", Vector3D::null, Rotation3D::null);
	
	Factory fab(telescope_config);
	fab.add_telescope_to_frame(telescope);

	sub_pixels = fab.get_sub_pixels();
	PhotonSensors::reset_all_sesnors(sub_pixels);
}
//------------------------------------------------------------------------------
void Calibration::set_up_telescope_environment() {

	settings.stores_only_last_intersection();
	telescope_environment.world_geometry = telescope;
	telescope_environment.propagation_options = &settings;
	telescope_environment.random_engine = &prng;		
}
//------------------------------------------------------------------------------
Photon* Calibration::get_photon_given_pos_and_angle_on_principal_aperture(
	Vector3D pos_on_principal_aperture,
	Vector3D direction_on_principal_aperture
)const {

	Ray back_running_ray(
		pos_on_principal_aperture, 
		direction_on_principal_aperture
	);

	Vector3D support_of_photon = back_running_ray.PositionOnRay(
		distance_to_travel_before_intersecting_principal_aperture
	);

	Photon* ph = new Photon(
		support_of_photon,
		direction_on_principal_aperture*(-1.0),
		callibration_photon_wavelenght
	);

	return ph;
}
//------------------------------------------------------------------------------
void Calibration::fill_calibration_block_to_table() {
	
	uint i;
	//int HadCatch = 0;

	#pragma omp parallel
	{
		#pragma omp for schedule(dynamic) private(i)
		for(i=0; i<number_of_photons_per_block; i++) {

			// create photon
			Vector3D pos_on_principal_aperture = 
				prng.get_point_on_xy_disc_within_radius(
					max_principal_aperture_radius_to_trow_photons_on
				);

			Vector3D direction_on_principal_aperture = 
				prng.get_point_on_unitsphere_within_polar_distance(
					max_tilt_vs_optical_axis_to_throw_photons_in
				);

			Photon* ph = get_photon_given_pos_and_angle_on_principal_aperture(
				pos_on_principal_aperture,
				direction_on_principal_aperture
			);

			// propagate photon
			ph->propagate_in(&telescope_environment);
			PhotonSensors::FindSensor sensor_finder(ph, sub_pixels);

			if(sensor_finder.is_absorbed_by_known_sensor()) {

				// remember photon
				CalibRow row;
				row.sub_pixel_id = sensor_finder.get_sensor()->get_id();
				row.x_pos_on_principal_aperture = pos_on_principal_aperture.x();
				row.y_pos_on_principal_aperture = pos_on_principal_aperture.y();
				row.x_tilt_vs_optical_axis = direction_on_principal_aperture.x();
				row.y_tilt_vs_optical_axis = direction_on_principal_aperture.y();
				row.relative_time_of_flight = ph->get_time_of_flight();

				table.push_back(row);
			}

			// delete photon
			ph->delete_history();
			delete ph;
		}
	}
}
//------------------------------------------------------------------------------
void Calibration::run_calibration() {

	std::cout << "start light field calibration" << "\n";

	for(uint j=0; j<number_of_blocks; j++) {
		std::cout << j+1 << " of " << number_of_blocks << "\n";

		table.clear();
		table.reserve(number_of_photons_per_block);
		
		fill_calibration_block_to_table();

		CalibIo appender("the_big_lebowsky.lftc");
		appender.append(table);	
	}
}
//------------------------------------------------------------------------------
std::string Calibration::get_print()const {

	std::stringstream out;
	out << "Light_Field_Calibration__\n";

	std::stringstream tab;
	tab << "number of photons........................ " << number_of_photons << "\n";
	tab << "principal aperture illumunation radius... " << max_principal_aperture_radius_to_trow_photons_on << "m\n";
	tab << "principal aperture illumunation angle.... " << Rad2Deg(max_tilt_vs_optical_axis_to_throw_photons_in) << "deg\n";
	tab << "distance for photon to travel before\n";
	tab << "reaching principal aperture plane........ " << distance_to_travel_before_intersecting_principal_aperture << "m\n";
	tab << "photon wavelength........................ " << callibration_photon_wavelenght << "m\n";
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		tab.str()
	);

	return out.str();
}
//------------------------------------------------------------------------------
} // namespace LightFieldTelescope