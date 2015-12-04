//=================================
// include guard
#ifndef __LightFieldTelescopeCalibIo_H_INCLUDED__
#define __LightFieldTelescopeCalibIo_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/Calibration.h"

namespace LightFieldTelescope {

	class CalibIo {

		const std::string path;
		std::ofstream file;
	public:

		CalibIo(const std::string path);
		void append(const std::vector<CalibRow> &table);
		void append_header(const Config telescope_config);
	private:

		void write_row(const CalibRow &row);
		void assert_file_is_open();
		class CanNotReadFile : public TracerException {
			using TracerException::TracerException;
		};		
	};
}
#endif // __LightFieldTelescopeCalibIo_H_INCLUDED__ 
