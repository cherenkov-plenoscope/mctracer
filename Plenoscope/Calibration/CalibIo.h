//=================================
// include guard
#ifndef __LightFieldTelescopeCalibIo_H_INCLUDED__
#define __LightFieldTelescopeCalibIo_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/Calibration/Calibration.h"

namespace Plenoscope {

	class CalibIo {

		const std::string path;
		std::ofstream file;
	public:

		CalibIo(const std::string path);
		void append(const std::vector<CalibrationPhotonResult> &table);
	private:

		void clear_file();
		void write_row(const CalibrationPhotonResult &row);
		void assert_file_is_open();
		class CanNotReadFile : public TracerException {
			using TracerException::TracerException;
		};		
	};
}
#endif // __LightFieldTelescopeCalibIo_H_INCLUDED__ 
