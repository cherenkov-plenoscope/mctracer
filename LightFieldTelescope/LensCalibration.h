//=================================
// include guard
#ifndef __LightFieldTelescopeLensCalibration_H_INCLUDED__
#define __LightFieldTelescopeLensCalibration_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/LightFieldTelescope.h"
#include "Core/Random/Random.h"
#include "Disc.h"
#include "BiConvexLensHexBound.h"

namespace LightFieldTelescope {

	class LensCalibration: public Printable {

		Frame test_bench;
		BiConvexLensHexBound lens;
		Frame face_plate;
		Disc image_sensor;
		PhotonSensor::PerfectSensor* read_out;
		PhotonSensors::Sensors read_out_l;

		TracerSettings settings;
		PropagationEnvironment test_bench_environment;
		Random::Mt19937 prng;
	public:

		const Config telescope_config;
		const Geometry telescope_geometry;
		
		LensCalibration(const Config cfg);
		~LensCalibration();
		std::string get_print()const;
	private:
		void set_up_test_bench();
	};
}
#endif // __LightFieldTelescopeLensCalibration_H_INCLUDED__ 
