#include "Calibrator.h"
#include "Plenoscope/Calibration/Writer.h"
#include "Tools/UserInteraction.h"
#include <iomanip> 
#include "Tools/FileTools.h"
#include "Core/PhotonAndFrame.h"
#include "Tools/HeaderBlock.h"
#include "omp.h"
//#include "Cameras/FlyingCamera.h"
using std::cout;

namespace Plenoscope {
	namespace Calibration {
//------------------------------------------------------------------------------
Calibrator::Calibrator(
	const Config _calib_config,
	PlenoscopeInScenery *_plenoscope, 
	const Frame* _scenery
):  
	config(_calib_config),
	scenery(_scenery),
	plenoscope(_plenoscope),
	lixel_statistics_filler(&plenoscope->light_field_sensor_geometry, &_calib_config)
{
	number_of_photons = config.photons_per_block*config.number_of_blocks;

	set_up_photon_properties();
	set_up_principal_aperture_range();
	set_up_field_of_view_range();
	set_up_plenoscope_environment();
	run_calibration();
}
//------------------------------------------------------------------------------
void Calibrator::set_up_photon_properties() {

	distance_to_travel_before_intersecting_principal_aperture = 1e4;
	callibration_photon_wavelenght = 433e-9;
}
//------------------------------------------------------------------------------
void Calibrator::set_up_principal_aperture_range() {

	max_principal_aperture_radius_to_trow_photons_on = 1.05*
		plenoscope->light_field_sensor_geometry.
			expected_imaging_system_max_aperture_radius();
}
//------------------------------------------------------------------------------
void Calibrator::set_up_field_of_view_range() {

	max_tilt_vs_optical_axis_to_throw_photons_in = 1.05*
		plenoscope->light_field_sensor_geometry.max_FoV_radius();
}
//------------------------------------------------------------------------------
void Calibrator::set_up_plenoscope_environment() {

	plenoscope_environment.world_geometry = scenery;
	plenoscope_environment.propagation_options = &settings;
	plenoscope_environment.random_engine = &prng;		
}
//------------------------------------------------------------------------------
Photon* Calibrator::get_photon_given_pos_and_angle_on_principal_aperture(
	Vec3 pos_on_principal_aperture,
	Vec3 direction_on_principal_aperture
)const {

	Ray back_running_ray(
		pos_on_principal_aperture, 
		direction_on_principal_aperture
	);

	Vec3 support_of_photon = back_running_ray.get_pos_at(
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
void Calibrator::fill_calibration_block_to_table() {
	
	uint i;
	int HadCatch = 0;

	#pragma omp parallel shared(HadCatch)
	{
		Random::Mt19937 thread_local_prng(
			prng.get_seed() + omp_get_thread_num()
		);
		#pragma omp for schedule(dynamic) private(i) 
		for(i=0; i<config.photons_per_block; i++) {

			try{
				// create photon
				Vec3 pos_on_principal_aperture = 
					thread_local_prng.get_point_on_xy_disc_within_radius(
						max_principal_aperture_radius_to_trow_photons_on
					);

				Vec3 direction_on_principal_aperture = 
					thread_local_prng.get_point_on_unitsphere_within_polar_distance(
						max_tilt_vs_optical_axis_to_throw_photons_in
					);

				Photon* ph = get_photon_given_pos_and_angle_on_principal_aperture(
					pos_on_principal_aperture,
					direction_on_principal_aperture
				);

				// propagate photon
				PhotonAndFrame::Propagator(ph, plenoscope_environment);

				PhotonSensors::FindSensorByFrame sensor_finder(
					ph->get_final_intersection().get_object(),
					&plenoscope->light_field_channels->by_frame
				);

				if(sensor_finder.frame_is_in_sensors()) {

					// remember photon
					CalibrationPhotonResult result;
					result.reached_sensor = true;
					result.lixel_id = sensor_finder.get_sensor()->get_id();
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
void Calibrator::run_calibration() {

	cout << "Plenoscope Calibrator: propagating ";
	cout << double(number_of_photons)/1.0e6 << "Mega photons\n";
	
	photon_results.resize(config.photons_per_block);

	for(uint j=0; j<config.number_of_blocks; j++) {
		
		cout << j+1 << " of " << config.number_of_blocks << "\n";	
		fill_calibration_block_to_table();
		lixel_statistics_filler.fill_in_block(photon_results);
	}
}
//------------------------------------------------------------------------------
void Calibrator::write_lixel_statistics(const string &path)const {
	vector<LixelStatistic> lixel_statistics =
		lixel_statistics_filler.get_lixel_statistics();

	write(lixel_statistics, path);	
}
//------------------------------------------------------------------------------
std::string Calibrator::get_print()const {

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
	}//Calibration
}//Plenoscope