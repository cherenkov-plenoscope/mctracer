#include "LixelStatistics.h"
#include "Tools/FileTools.h"

namespace Plenoscope {
    namespace Calibration {
//------------------------------------------------------------------------------
LixelStatistics::LixelStatistics(
	const LightFieldSensor::Geometry *geometry,  
	const Config *_calib_config
):
	calib_config(_calib_config),
	sensor_geometry(geometry),
	photons_emitted_per_lixel(
		double(_calib_config->number_of_blocks) * double(_calib_config->photons_per_block) /
		double(geometry->number_of_lixel())
	)
 {
	lixel_stats.resize(sensor_geometry->number_of_lixel());
}
//------------------------------------------------------------------------------
void LixelStatistics::fill_in_block(vector<CalibrationPhotonResult> &calib_block) {

	for(CalibrationPhotonResult result: calib_block) {
		if(result.reached_sensor == true) {
			lixel_stats[result.lixel_id].count++;
			lixel_stats[result.lixel_id].cx.add(result.x_tilt_vs_optical_axis);
			lixel_stats[result.lixel_id].cy.add(result.y_tilt_vs_optical_axis);
			lixel_stats[result.lixel_id].x.add(result.x_pos_on_principal_aperture);
			lixel_stats[result.lixel_id].y.add(result.y_pos_on_principal_aperture);
			lixel_stats[result.lixel_id].time.add(result.relative_time_of_flight);
		}
	}
}
//------------------------------------------------------------------------------
double LixelStatistics::min_arrival_time_mean()const {

	double minimal_time_mean = 0.0;
	if(lixel_stats.size() > 0)
		minimal_time_mean = lixel_stats.at(0).time.mean();

	for(LixelStatistic lixel: lixel_stats) {
		double this_pix_time = lixel.time.mean();
		if(this_pix_time < minimal_time_mean)
			minimal_time_mean = this_pix_time;
	}
	return minimal_time_mean;
}
//------------------------------------------------------------------------------
void LixelStatistics::save(const std::string path) {
		
	const double minimal_arrival_time = min_arrival_time_mean();

	std::stringstream out;
	out << "# __Plenoscope Bin Statistics__\n";
	out << "# pixel: " << sensor_geometry->number_of_pixels() << "\n";
	out << "# paxel: " << sensor_geometry->number_of_paxel_per_pixel() << "\n";
	out << "# photons_emitted_per_lixel: " << photons_emitted_per_lixel << "\n";
	out << "# geometrical_efficiency[1]\t";
	out << "cx[rad]\t";
	out << "cx_std[rad]\t";
	out << "cy[rad]\t";
	out << "cy_std[rad]\t";
	out << "x[m]\t";
	out << "x_std[m]\t";
	out << "y[m]\t";
	out << "y_std[m]\t";
	out << "t[s]\t";
	out << "t_std[s]\n";
	out.precision(4);
	
	for(LixelStatistic lixel: lixel_stats) {
		out << lixel.count/photons_emitted_per_lixel << "\t";
		out << lixel.cx.mean() << "\t";
		out << lixel.cx.stddev() << "\t";
		out << lixel.cy.mean() << "\t";
		out << lixel.cy.stddev() << "\t";
		out << lixel.x.mean() << "\t";
		out << lixel.x.stddev() << "\t";
		out << lixel.y.mean() << "\t";
		out << lixel.y.stddev() << "\t";
		out << lixel.time.mean() - minimal_arrival_time << "\t";
		out << lixel.time.stddev() << "\n";
	}

	FileTools::write_text_to_file(out.str(), path);
}
//------------------------------------------------------------------------------
    }//Calibration
}//Plenoscope