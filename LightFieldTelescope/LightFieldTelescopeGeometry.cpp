#include "LightFieldTelescope.h"
#include "HexGridAnnulus.h"
#include "LensMaker.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Geometry::Geometry(const Config ncfg): 
	cfg(ncfg),
	reflector(ncfg.reflector)
{
	HexGridAnnulus pixgrid(
		max_outer_sensor_radius() - pixel_lens_outer_aperture_radius(), 
		pixel_spacing()
	);

	pixel_grid =  pixgrid.get_grid();
	
	HexGridAnnulus subpixgrid(
		sub_pixel_sensor_radius(),
		sub_pixel_spacing()
	);

	sub_pixel_grid = subpixgrid.get_grid();

	pixel_lens_mean_refrac = cfg.lens_refraction->get_mean(137);

	LensMaker::Config lmcfg;
	lmcfg.focal_length = pixel_lens_focal_length();
	lmcfg.aperture_radius = pixel_lens_outer_aperture_radius();
	lmcfg.refractive_index = pixel_lens_mean_refrac;

	pixel_lens_curv_radius = 
		LensMaker::Approximation::get_curvature_radius(lmcfg);
}
//------------------------------------------------------------------------------
std::string Geometry::get_print()const{
	std::stringstream out;
	out << "Light_Field_Telescope__\n";
	out << "\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		reflector.get_print()
	);
	out << "\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		get_image_sensor_print()
	);

	return out.str();		
}
//------------------------------------------------------------------------------
std::string Geometry::get_image_sensor_print()const{

	std::stringstream out;
	out << "Image_Sensor__\n";

	std::stringstream tab;
	tab << "Field of View................. " << Rad2Deg(cfg.max_FoV_diameter) << "deg\n";
	tab << "max sensor radius............. " << max_outer_sensor_radius() << "m\n";
	tab << "number of pixels.............. " << number_of_pixels() << "\n";
	tab << "pixel FoV hex flat2flat....... " << Rad2Deg(pixel_FoV_hex_flat2flat()) << "deg\n";
	tab << "pixel spacing................. " << pixel_spacing() << "m\n";
	tab << "pixel inner aperture radius... " << pixel_lens_inner_aperture_radius() << "m\n";
	tab << "pixel outer aperture radius... " << pixel_lens_outer_aperture_radius() << "m\n";
	tab << "pixel lens f over D........... " << pixel_lens_f_over_D() << "\n";
	tab << "pixel lens focal length....... " << pixel_lens_focal_length() << "m\n";
	tab << "pixel lens curvature radius... " << pixel_lens_curvature_radius() << "m\n";
	tab << "pixel lens refraction mean.... " << pixel_lens_mean_refraction() << "\n";
	tab << "sub pixel sensor radius....... " << sub_pixel_sensor_radius() << "m\n";
	tab << "sub pixel per pixel........... " << sub_pixel_per_pixel() << "\n";
	tab << "sub pixel total number........ " << sub_pixel_grid.size() << "\n";
	tab << "sub pixel outer radius........ " << sub_pixel_outer_radius() << "m\n";
	tab << "sub pixel inner radius........ " << sub_pixel_inner_radius() << "m\n";
	tab << "sub pixel spacing............. " << sub_pixel_spacing() << "m\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		tab.str()
	);

	return out.str();
}
//------------------------------------------------------------------------------
double Geometry::max_outer_sensor_radius()const {
	return reflector.focal_length()*tan(max_FoV_radius());
}
//------------------------------------------------------------------------------
double Geometry::max_FoV_radius()const {
	return cfg.max_FoV_diameter/2.0;
}
//------------------------------------------------------------------------------
uint Geometry::number_of_pixels()const {
	return pixel_grid.size();
}
//------------------------------------------------------------------------------
double Geometry::pixel_spacing()const {
	return pixel_lens_inner_aperture_radius()*2.0;
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_inner_aperture_radius()const {
	return reflector.focal_length()*tan(cfg.pixel_FoV_hex_flat2flat/2.0);
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_outer_aperture_radius()const {
	return pixel_lens_inner_aperture_radius()*2.0/sqrt(3.0);
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_focal_length()const {
	return pixel_lens_f_over_D()*2.0*pixel_lens_outer_aperture_radius();
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_f_over_D()const {
	return reflector.naive_f_over_D();
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_curvature_radius()const {
	return pixel_lens_curv_radius;
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_mean_refraction()const {
	return pixel_lens_mean_refrac;
}
//------------------------------------------------------------------------------
double Geometry::pixel_FoV_hex_flat2flat()const {
	return cfg.pixel_FoV_hex_flat2flat;
}
//------------------------------------------------------------------------------
double Geometry::outer_sensor_housing_radius()const {
	return max_outer_sensor_radius()*cfg.housing_overhead;
}
//------------------------------------------------------------------------------
std::vector<Vector3D> Geometry::pixel_positions()const {
	return pixel_grid;
}
//------------------------------------------------------------------------------
std::vector<Vector3D> Geometry::sub_pixel_positions()const {
	return sub_pixel_grid;
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_per_pixel()const {
	return cfg.sub_pixel_per_pixel;
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_sensor_radius()const {

	return max_outer_sensor_radius() + 
		pixel_lens_focal_length()/reflector.focal_length() *
		(reflector.max_outer_aperture_radius() + max_outer_sensor_radius());
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_outer_radius()const {
	return pixel_lens_outer_aperture_radius()/sqrt(sub_pixel_per_pixel());
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_inner_radius()const {
	return sub_pixel_outer_radius()*sqrt(3.0)/2.0;
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_spacing()const {
	return sub_pixel_inner_radius()*2.0;
}
//------------------------------------------------------------------------------
} //lightfield