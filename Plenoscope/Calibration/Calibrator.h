//=================================
// include guard
#ifndef __PlenoscopeCalibrationCalibrator_H_INCLUDED__
#define __PlenoscopeCalibrationCalibrator_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/PlenoscopeInScenery.h"
#include "Plenoscope/Calibration/Config.h"
#include "Plenoscope/Calibration/CalibrationPhotonResult.h"
#include "Plenoscope/Calibration/LixelStatisticsFiller.h"
#include "Core/Random/Random.h"
#include "Core/PropagationEnvironment.h"

namespace Plenoscope {
    namespace Calibration {

	class Calibrator: public Printable {

		const Config config;
		const Frame* scenery; 
		PlenoscopeInScenery* plenoscope;
		
		uint number_of_photons;
		double max_principal_aperture_radius_to_trow_photons_on;
		double max_tilt_vs_optical_axis_to_throw_photons_in;
		double distance_to_travel_before_intersecting_principal_aperture;
		double callibration_photon_wavelenght;

		PropagationConfig settings;
		PropagationEnvironment plenoscope_environment;
		Random::Mt19937 prng;

		vector<CalibrationPhotonResult> photon_results;
		LixelStatisticsFiller lixel_statistics_filler;
	public:

		Calibrator(
			const Config _calib_config,
			PlenoscopeInScenery *plenoscope, 
			const Frame* _scenery
		);
		void export_sub_pixel_statistics(const string path)const;
		void write_lixel_statistics(const string &path)const;
		string get_print()const;
	private:

		void set_up_photon_properties();
		void set_up_principal_aperture_range();
		void set_up_field_of_view_range();
		void set_up_plenoscope_environment();
		void run_calibration();
		void fill_calibration_block_to_table();
		Photon get_photon_given_pos_and_angle_on_principal_aperture(
			Vec3 pos_on_principal_aperture,
			Vec3 direction_on_principal_aperture
		)const;
	};

    }//Calibration
}//Plenoscope
#endif // __PlenoscopeCalibrationCalibrator_H_INCLUDED__ 
