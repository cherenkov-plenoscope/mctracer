//=================================
// include guard
#ifndef __LightFieldTelescopeCalibration_H_INCLUDED__
#define __LightFieldTelescopeCalibration_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/Geometry.h"
#include "LightFieldTelescope/Factory.h"
#include "LightFieldTelescope/Calibration/Config.h"
#include "LightFieldTelescope/Calibration/CalibrationPhotonResult.h"
#include "LightFieldTelescope/Calibration/SubPixelStatistics.h"
#include "Core/Random/Random.h"

namespace LightFieldTelescope {

	class Calibration: public Printable {

		const CalibrationConfig *config;
		const Geometry *telescope_geometry;
		uint number_of_photons;
		double max_principal_aperture_radius_to_trow_photons_on;
		double max_tilt_vs_optical_axis_to_throw_photons_in;
		double distance_to_travel_before_intersecting_principal_aperture;
		double callibration_photon_wavelenght;

		Frame* telescope;
		PhotonSensors::Sensors *sub_pixels;

		TracerSettings settings;
		PropagationEnvironment telescope_environment;
		Random::Mt19937 prng;

		std::vector<CalibrationPhotonResult> photon_results;

		SubPixelStatistics stats;
	public:

		Calibration(const Geometry *geometry, const CalibrationConfig *config);
		void export_sub_pixel_statistics(const std::string path)const;
		std::string get_print()const;
	private:

		void set_up_photon_properties();
		void set_up_principal_aperture_range();
		void set_up_field_of_view_range();
		void set_up_telescope();
		void set_up_telescope_environment();
		void run_calibration();
		void fill_calibration_block_to_table();
		Photon* get_photon_given_pos_and_angle_on_principal_aperture(
			Vector3D pos_on_principal_aperture,
			Vector3D direction_on_principal_aperture
		)const;
	};
}
#endif // __LightFieldTelescopeCalibration_H_INCLUDED__ 
