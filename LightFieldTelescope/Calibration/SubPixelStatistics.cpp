#include "SubPixelStatistics.h"
#include "Tools/FileTools.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
SubPixelStatistics::SubPixelStatistics(
	const Geometry *geometry,  
	const CalibrationConfig *_calib_config
):
	calib_config(_calib_config),
	telescope_geometry(geometry)
 {
	subpix_stats.resize(telescope_geometry->total_number_of_sub_pixels());
}
//------------------------------------------------------------------------------
void SubPixelStatistics::fill_in_block(std::vector<CalibrationPhotonResult> &calib_block) {

	for(CalibrationPhotonResult result: calib_block) {
		if(result.reached_sensor == true) {
			subpix_stats[result.sub_pixel_id].geometric_efficiency += 1.0;
			subpix_stats[result.sub_pixel_id].mean_cx += result.x_tilt_vs_optical_axis;
			subpix_stats[result.sub_pixel_id].mean_cy += result.y_tilt_vs_optical_axis;
			subpix_stats[result.sub_pixel_id].mean_x += result.x_pos_on_principal_aperture;
			subpix_stats[result.sub_pixel_id].mean_y += result.y_pos_on_principal_aperture;
			subpix_stats[result.sub_pixel_id].mean_time += result.relative_time_of_flight;
		}
	}
}
//------------------------------------------------------------------------------
void SubPixelStatistics::normalize() {
	
	double min_time = subpix_stats.front().mean_time;
	for(uint i=0; i<subpix_stats.size(); i++) {

		subpix_stats[i].mean_cx /= subpix_stats[i].geometric_efficiency;
		subpix_stats[i].mean_cy /= subpix_stats[i].geometric_efficiency;
		subpix_stats[i].mean_x /= subpix_stats[i].geometric_efficiency;
		subpix_stats[i].mean_y /= subpix_stats[i].geometric_efficiency;
		subpix_stats[i].mean_time /= subpix_stats[i].geometric_efficiency;
		subpix_stats[i].geometric_efficiency /= calib_config->photons_per_sub_pixel;
	
		if(	subpix_stats[i].mean_time < min_time)
			min_time = subpix_stats[i].mean_time;
	}

	for(uint i=0; i<subpix_stats.size(); i++)
		subpix_stats[i].mean_time -= min_time;
}
//------------------------------------------------------------------------------
void SubPixelStatistics::save(const std::string path) {
		
	normalize();
		
	std::stringstream out;
	out << "# sub pixel subpix_stats:\n";
	out << "# number_of_photons_per_sub_pixel: ";
	out << calib_config->photons_per_sub_pixel << "\n";
	out << "# geometrical_efficiency[1]\tcx[rad]\tcy[rad]\tx[m]\ty[m]\tt[s]\n";
	out.precision(4);
	
	for(SubPixelStatistic pix: subpix_stats) {

		out << pix.geometric_efficiency << "\t";
		out << pix.mean_cx << "\t";
		out << pix.mean_cy << "\t";
		out << pix.mean_x << "\t";
		out << pix.mean_y << "\t";
		out << pix.mean_time << "\n";
	}

	FileTools::write_text_to_file(out.str(), path);
}
//------------------------------------------------------------------------------
} // namespace LightFieldTelescope