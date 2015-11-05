#include "Cameras/SkyDome.h"
#include <sstream>
#include "math.h"
//------------------------------------------------------------------------------
SkyDome::SkyDome(const std::string _filename) {
	filename = _filename;
	sky.load(filename);
	central_row = sky.get_number_of_rows()/2;
	central_col = sky.get_number_of_cols()/2;
	zenith_to_horizon_radius = 
		central_row<central_col ? central_row : central_col;
}
//------------------------------------------------------------------------------
Color SkyDome::get_color_for_direction(Vector3D dir)const {
	
	dir.normalize();

	if(dir.z() < 0.0)
		return Color::gray;
	else
		return sky_dome_color_for(dir);
}
//------------------------------------------------------------------------------
Color SkyDome::sky_dome_color_for(const Vector3D dir)const {
	const double zd = get_zenith_distance_of(dir);
	const double az = get_azimuth_angle_of(dir);

	const double zd_in_pix = zd*two_over_pi*zenith_to_horizon_radius;

	const uint col_offset = round(zd_in_pix*cos(az));
	const uint row_offset =-round(zd_in_pix*sin(az));

	const uint col = central_col + col_offset;
	const uint row = central_row + row_offset;

	return sky.get_pixel_row_col(row, col);
}
//------------------------------------------------------------------------------
std::string SkyDome::get_print()const {
	std::stringstream out;
	out << "SkyDome " << sky.get_number_of_cols() << "x" << sky.get_number_of_rows();
	out << " " << filename << "\n";
	out << "central pixel (" << central_col << "," central_row <<"), radius " << zenith_to_horizon_radius << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
double SkyDome::get_zenith_distance_of(const Vector3D dir)const {
	return Vector3D::unit_z.get_angle_in_between_in_rad(dir);
}
//------------------------------------------------------------------------------
double SkyDome::get_azimuth_angle_of(const Vector3D dir)const {
	Vector3D projection_in_xy_plane(dir.x(), dir.y(), 0.0);
	return Vector3D::unit_x.get_angle_in_between_in_rad(projection_in_xy_plane);
}
//------------------------------------------------------------------------------