//=================================
// include guard
#ifndef __PlenoscopeCalibrationWriter_H_INCLUDED__
#define __PlenoscopeCalibrationWriter_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/Calibration/Calibration.h"

namespace Plenoscope {
namespace Calibration {

	class Writer {

		const std::string path;
		std::ofstream file;
	public:

		Writer(const std::string path);
		void append(const std::vector<CalibrationPhotonResult> &table);
	private:

		void clear_file();
		void write_row(const CalibrationPhotonResult &row);
		void assert_file_is_open();
		class CanNotReadFile : public TracerException {
			using TracerException::TracerException;
		};		
	};
}//Calibration
}//Plenoscope
#endif // __PlenoscopeCalibrationWriter_H_INCLUDED__ 
