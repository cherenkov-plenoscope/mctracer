#include "Calibration.h"
#include "CalibIo.h"
#include "Tools/UserInteraction.h"
#include <iomanip> 
#include "Tools/FileTools.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Calibration::Calibration(
	const Geometry *geometry,  
	const CalibrationConfig *_calib_config
):  
	calib_config(_calib_config),
	telescope_geometry(geometry),
	stats(geometry, _calib_config)
{
	number_of_photons = telescope_geometry->total_number_of_sub_pixels()*
		calib_config->photons_per_sub_pixel;

	number_of_blocks = ceil(
		double(number_of_photons)/
		double(calib_config->photons_per_block)
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

	max_principal_aperture_radius_to_trow_photons_on = 
		telescope_geometry->principal_aperture_radius_to_throw_photons_in();
}
//------------------------------------------------------------------------------
void Calibration::set_up_field_of_view_range() {

	max_tilt_vs_optical_axis_to_throw_photons_in = 
		telescope_geometry->max_FoV_radius()*1.05;
}
//------------------------------------------------------------------------------
void Calibration::set_up_telescope() {

	telescope = new Frame("telescope", Vector3D::null, Rotation3D::null);
	
	Factory fab(telescope_geometry);
	fab.add_telescope_to_frame(telescope);

	sub_pixels = fab.get_sub_pixels();
	sub_pixels->clear_history();
}
//------------------------------------------------------------------------------
void Calibration::set_up_telescope_environment() {

	settings.store_only_final_intersection = true;
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
	int HadCatch = 0;

	#pragma omp parallel shared(HadCatch)
	{
		#pragma omp for schedule(dynamic) private(i) 
		for(i=0; i<calib_config->photons_per_block; i++) {

			try{
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
				ph->propagate_in(telescope_environment);
				
				PhotonSensors::FindSensorByFrame sensor_finder(
					ph->get_final_intersection()->get_object(),
					&sub_pixels->by_frame
				);

				if(sensor_finder.frame_is_in_sensors()) {

					// remember photon
					CalibrationPhotonResult result;
					result.reached_sensor = true;
					result.sub_pixel_id = sensor_finder.get_sensor()->get_id();
					result.x_pos_on_principal_aperture = pos_on_principal_aperture.x();
					result.y_pos_on_principal_aperture = pos_on_principal_aperture.y();
					result.x_tilt_vs_optical_axis = direction_on_principal_aperture.x();
					result.y_tilt_vs_optical_axis = direction_on_principal_aperture.y();
					result.relative_time_of_flight = ph->get_time_of_flight();

					photon_results[i] = result;
				}else{

					CalibrationPhotonResult result;
					result.reached_sensor = false;
					photon_results[i] = result;
				}

				// delete photon
				ph->delete_history();
				delete ph;
			}catch(std::exception &error) {

				HadCatch++;
				std::cerr << error.what(); 
			}catch(...)	{

				HadCatch++;
			}
		}
	}

	if(HadCatch) {
		std::stringstream info;
		info <<__FILE__<<", "<<__LINE__<<"\n";
		info << "Cought exception during multithread ";
		info << "Light Field Telescope Calibration.\n";
		throw(TracerException(info.str()));
	}
}
//------------------------------------------------------------------------------
void Calibration::run_calibration() {

	std::cout << telescope_geometry->get_print() << "\n";

	std::cout << "Start Light Field Calibration, propagating ";
	std::cout << double(
					number_of_blocks * calib_config->photons_per_block
				)/1.0e6;
	std::cout << "M photons\n";
	std::cout << "\n";
	
	photon_results.resize(calib_config->photons_per_block);

	for(uint j=0; j<number_of_blocks; j++) {
		
		std::cout << j+1 << " of " << number_of_blocks << "\n";	
		
		fill_calibration_block_to_table();
		stats.fill_in_block(photon_results);

		CalibIo appender("the_big_lebowsky.lftc");
		appender.append(photon_results);	
	}

	stats.save("sub_pixel_statistics.txt");
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