#include "LightFieldTelescope/CalibIo.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
CalibIo::CalibIo(const std::string npath): path(npath){}
//------------------------------------------------------------------------------
void CalibIo::append(const std::vector<CalibRow> &table) {

	file.open(path, std::ios::app | std::ios::binary);
	assert_file_is_open();

	for(CalibRow row : table)
		write_row(row);

	file.close();
}
//------------------------------------------------------------------------------
void CalibIo::write_row(const CalibRow &row) {
	
	// write ID integer
	file.write( (char*)&row.sub_pixel_id, sizeof(uint32_t));

	// write pos dir and time floats
	const uint float_count = 5;

	float v[float_count] = {
		row.x_pos_on_principal_aperture, 
		row.y_pos_on_principal_aperture,
		row.x_tilt_vs_optical_axis,
		row.y_tilt_vs_optical_axis,
		row.relative_time_of_flight
	};

	file.write( (char*)&v, float_count*sizeof(float));
}
//------------------------------------------------------------------------------
void CalibIo::assert_file_is_open() {
	
	if(!file.is_open()) {
		std::stringstream info;
	    info << __FILE__ << " " << __LINE__ << "\n";
		info << "Light Field Telescope Calibration: Unable to write to file: '";
		info << path << "'\n";
		throw CanNotReadFile(info.str());	 	
	}	
}
//------------------------------------------------------------------------------
}// namespace LightFieldTelescope