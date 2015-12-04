//=================================
// include guard
#ifndef __LightFieldTelescopeLensCalibration_H_INCLUDED__
#define __LightFieldTelescopeLensCalibration_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/Telescope.h"
#include "Core/PseudoRandomNumberGenerator.h"
#include "Disc.h"
#include "BiConvexLensHexBound.h"

namespace LightFieldTelescope {

	class LensCalibration: public Printable {

		const Config telescope_config;
		const Geometry telescope_geometry;

		Frame test_bench;
		BiConvexLensHexBound lens;
		Frame face_plate;
		Disc image_sensor;
		PhotonSensor::PerfectSensor* read_out;
		std::vector<PhotonSensor::Sensor*> read_out_list;

		TracerSettings settings;
		PropagationEnvironment test_bench_environment;
		Random::Mt19937 prng;
	public:

		LensCalibration(const Config cfg);
		~LensCalibration();
		std::string get_print()const;
	private:
		void set_up_test_bench();
	};
}
#endif // __LightFieldTelescopeLensCalibration_H_INCLUDED__ 
