#include "Cameras/SkyDome.h"
#include <sstream>
#include "math.h"

using namespace SkyDome;

//const Monochrom blue_sky = Monochrom(Color::sky_blue);
//------------------------------------------------------------------------------
double Dome::get_zenith_distance_of(const Vector3D dir)const {
	return Vector3D::unit_z.get_angle_in_between_in_rad(dir);
}
//------------------------------------------------------------------------------
double Dome::get_azimuth_angle_of(const Vector3D dir)const {
	return atan2(dir.y(),dir.x());
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
Texture::Texture(const std::string _filename):sky(CameraImage(_filename)){
	filename = _filename;

	central_row = sky.get_number_of_rows()/2;
	central_col = sky.get_number_of_cols()/2;
	zenith_to_horizon_radius = 
		central_row<central_col ? central_row : central_col;
}
//------------------------------------------------------------------------------
Color Texture::get_color_for_direction(Vector3D dir)const {

	if(dir.z() < 0.0)
		return Color::black;
	else
		return sky_dome_color_for(dir);
}
//------------------------------------------------------------------------------
Color Texture::sky_dome_color_for(const Vector3D dir)const {
	const double zd = get_zenith_distance_of(dir);
	const double az = get_azimuth_angle_of(dir);

	const double zd_in_pix = zd*two_over_pi*zenith_to_horizon_radius;

	// round towards 0 (by calling (int) instead of rounding)
	// to prevent segfault when index out of bounds
	const int col_offset = (int)(zd_in_pix*cos(az)); 
	const int row_offset =-(int)(zd_in_pix*sin(az)); 

	const uint col = central_col + col_offset;
	const uint row = central_row + row_offset;

	return sky.get_pixel_row_col(row, col);
}
//------------------------------------------------------------------------------
std::string Texture::get_print()const {
	std::stringstream out;
	out << "SkyDome " << sky.get_number_of_cols() << "x" << sky.get_number_of_rows();
	out << " " << filename << ", ";
	out << "central pixel (" << central_col << "," ;
	out << central_row <<"), radius " << zenith_to_horizon_radius;
	return out.str();
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
Monochrom::Monochrom(const Color _dome_color) {
	dome_color = _dome_color;
}
//------------------------------------------------------------------------------
Monochrom::Monochrom() :
	dome_color(Color::sky_blue){}
//------------------------------------------------------------------------------
Color Monochrom::get_color_for_direction(const Vector3D dir)const {
	return Color::sky_blue;
}
//------------------------------------------------------------------------------
std::string Monochrom::get_print()const {
	std::stringstream out;
	out << "SkyDome Monochrome " << dome_color;
	return out.str();
}
//------------------------------------------------------------------------------
//const SkyDome::Monochrom blue_sky = SkyDome::Monochrom(Color::sky_blue);