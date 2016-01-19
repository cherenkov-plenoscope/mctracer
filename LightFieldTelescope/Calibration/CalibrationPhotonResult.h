//=================================
// include guard
#ifndef __LightFieldTelescopeCalibrationPhotonResult_H_INCLUDED__
#define __LightFieldTelescopeCalibrationPhotonResult_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <stdint.h>

namespace LightFieldTelescope {

	struct CalibrationPhotonResult {
		bool reached_sensor;
		uint32_t sub_pixel_id;
		float x_pos_on_principal_aperture;
		float y_pos_on_principal_aperture;
		float x_tilt_vs_optical_axis;
		float y_tilt_vs_optical_axis;
		float relative_time_of_flight;

		CalibrationPhotonResult();
	};
} //LightFieldTelescope
#endif // __LightFieldTelescopeCalibrationPhotonResult_H_INCLUDED__ 


