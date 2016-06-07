#include "Config.h"

namespace Plenoscope {
    namespace Calibration {
//------------------------------------------------------------------------------
Config::Config() {
	number_of_blocks = 8;
	photons_per_block = 1e6;
	raw_calibration_output_path = "raw_lightfield_calibration.bin";
	condensed_calibration_output_path = "condensed_lightfield_calibration.txt";
}
//------------------------------------------------------------------------------
	}//Calibration
}//Plenoscope