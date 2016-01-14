#include "LightFieldTelescope/Geometry.h"
#include "Geometry/HexGridAnnulus.h"
#include "Tools/AsciiIo.h"
#include "Geometry/HexGridFlower.h"
#include "LensMaker/LensMaker.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Geometry::Geometry(const Config ncfg): 
	config(ncfg),
	reflector(ncfg.reflector)
{
	set_up_pixel_grid();
	set_up_flower_grid();
	set_up_sub_pixel_flower_template_grid();
	set_up_sub_pixel_grid();
	set_up_lens_geometry();
}
//------------------------------------------------------------------------------
void Geometry::set_up_pixel_grid() {

	HexGridAnnulus pixgrid(
		max_outer_sensor_radius() - pixel_lens_outer_aperture_radius(), 
		pixel_spacing()
	);

	pixel_grid =  pixgrid.get_grid();	
}
//------------------------------------------------------------------------------
void Geometry::set_up_sub_pixel_flower_template_grid() {

	HexGridFlower subpixflowergrid(
		pixel_lens_outer_aperture_radius(),
		config.sub_pixel_on_pixel_diagonal
	);

	sub_pixel_flat2flat = subpixflowergrid.get_facet_spacing();
	sub_pixel_flower_template_grid = subpixflowergrid.get_grid();
}
//------------------------------------------------------------------------------
void Geometry::set_up_flower_grid() {

	sub_pixel_flower_grid.reserve(pixel_grid.size());

	const double r = 1.0 + pixel_lens_sub_pixel_distance()/image_sensor_distance();

	for(Vector3D pixel_pos : pixel_grid)
		sub_pixel_flower_grid.push_back(
			pixel_pos*r
		);
}
//------------------------------------------------------------------------------
void Geometry::set_up_sub_pixel_grid() {

	sub_pixel_grid.reserve(
		sub_pixel_flower_grid.size()*
		sub_pixel_flower_template_grid.size()
	);

	for(Vector3D flower_pos : sub_pixel_flower_grid)
		for(Vector3D sub_pixel_pos : sub_pixel_flower_template_grid)
			sub_pixel_grid.push_back(flower_pos + sub_pixel_pos);
}
//------------------------------------------------------------------------------
void Geometry::set_up_lens_geometry() {
	
	pixel_lens_mean_refrac = config.lens_refraction->get_mean(137);

	LensMaker::Config lmcfg;
	lmcfg.focal_length = pixel_lens_focal_length();
	lmcfg.aperture_radius = pixel_lens_outer_aperture_radius();
	lmcfg.refractive_index = pixel_lens_mean_refrac;

	double lens_maker_correction = 1.20;

	pixel_lens_curv_radius = lens_maker_correction*
		LensMaker::Approximation::get_curvature_radius(lmcfg);
}
//------------------------------------------------------------------------------
double Geometry::max_outer_sensor_radius()const {
	return reflector.focal_length()*tan(max_FoV_radius());
}
//------------------------------------------------------------------------------
double Geometry::image_sensor_distance()const {
	return ThinLensEquation::get_image_dist_given_focal_and_object_dist(
		reflector.focal_length(),
		config.object_distance_to_focus_on
	);
}
//------------------------------------------------------------------------------
double Geometry::max_FoV_radius()const {
	return config.max_FoV_diameter/2.0;
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
	return reflector.focal_length()*tan(config.pixel_FoV_hex_flat2flat/2.0);
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_outer_aperture_radius()const {
	return pixel_lens_inner_aperture_radius()*2.0/sqrt(3.0);
}
//------------------------------------------------------------------------------
double Geometry::aperture_image_radius_on_sub_pixel_sensor()const {
	return pixel_lens_inner_aperture_radius() - 0.5*sub_pixel_inner_radius();
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_focal_length()const {
	return reflector.naive_f_over_D()*2.0*aperture_image_radius_on_sub_pixel_sensor();
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_sub_pixel_distance()const {
	return pixel_lens_focal_length();
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_f_over_D()const {
	return pixel_lens_focal_length()/(2.0*pixel_lens_outer_aperture_radius());
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
	return config.pixel_FoV_hex_flat2flat;
}
//------------------------------------------------------------------------------
double Geometry::outer_sensor_housing_radius()const {
	return max_outer_sensor_radius()*config.housing_overhead;
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
std::vector<Vector3D> Geometry::sub_pixel_flower_positions()const {
	return sub_pixel_flower_grid;
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_z_orientation()const {
	return 1.0/6.0*M_PI;
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_per_pixel()const {
	return sub_pixel_flower_template_grid.size();
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_outer_radius()const {
	return sub_pixel_inner_radius()*2.0/sqrt(3.0);
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_inner_radius()const {
	return 0.5*sub_pixel_spacing();
}
//------------------------------------------------------------------------------
double Geometry::sub_pixel_spacing()const {
	return sub_pixel_flat2flat;
}
//------------------------------------------------------------------------------
uint Geometry::total_number_of_sub_pixels()const {
	return sub_pixel_grid.size();
}
//------------------------------------------------------------------------------
double Geometry::bin_hight()const {
	return pixel_lens_sub_pixel_distance()*0.25;
}
//------------------------------------------------------------------------------
void Geometry::write_sub_pixel_positions(const std::string path)const {

	std::vector<std::vector<double>> sub_pixels_x_y;

	for(Vector3D pos : sub_pixel_grid) {
		std::vector<double> sub_pixel_xy = {pos.x(), pos.y()};
		sub_pixels_x_y.push_back(sub_pixel_xy);
	}

	std::stringstream header;
	header << "number_of_sub_pixels = " << sub_pixel_grid.size() << "\n";
	header << "number_of_sub_pixels_per_pixel = " << sub_pixel_per_pixel() << "\n";
	header << "sub_pixel_z_orientation = " << sub_pixel_z_orientation() << "\n";
	header << "sub_pixel_inner_radius = " << sub_pixel_inner_radius() << "\n";
	header << "sub_pixel_outer_radius = " << sub_pixel_outer_radius() << "\n";
	header << "x[m]\ty[m]\n";

	AsciiIo::write_table_to_file_with_header(
		sub_pixels_x_y,
		path,
		header.str()
	);
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
	out << "\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		get_pixel_lens_print()
	);
	out << "\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		get_sub_pixel_print()
	);
	out << "\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		get_concentrator_bin_print()
	);
	
	return out.str();		
}
//------------------------------------------------------------------------------
std::string Geometry::get_image_sensor_print()const{

	std::stringstream out;
	out << "Image_Sensor__\n";

	std::stringstream tab;
	tab << "Field of View................. " << Rad2Deg(config.max_FoV_diameter) << "deg\n";
	tab << "Field of View solid angle..... " << field_of_view_solid_angle() << " radians\n";
	tab << "max sensor radius............. " << max_outer_sensor_radius() << "m\n";
	tab << "number of pixels.............. " << number_of_pixels() << "\n";
	tab << "object distance to focus on... " << config.object_distance_to_focus_on << "m\n";
	tab << "image sensor distance......... " << image_sensor_distance() << "m\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		tab.str()
	);

	return out.str();
}
//------------------------------------------------------------------------------
std::string Geometry::get_pixel_lens_print()const{

	std::stringstream out;
	out << "Pixel_Lens__\n";

	std::stringstream tab;
	tab << "FoV hex flat2flat....... " << Rad2Deg(pixel_FoV_hex_flat2flat()) << "deg\n";
	tab << "spacing................. " << pixel_spacing() << "m\n";
	tab << "inner aperture radius... " << pixel_lens_inner_aperture_radius() << "m\n";
	tab << "outer aperture radius... " << pixel_lens_outer_aperture_radius() << "m\n";
	tab << "lens f over D........... " << pixel_lens_f_over_D() << "\n";
	tab << "lens focal length....... " << pixel_lens_focal_length() << "m\n";
	tab << "lens curvature radius... " << pixel_lens_curvature_radius() << "m\n";
	tab << "lens to sub pixel dist.. " << pixel_lens_sub_pixel_distance() << "m\n";
	tab << "lens refraction mean.... " << pixel_lens_mean_refraction() << "\n";
	tab << "aperture image radius... " << aperture_image_radius_on_sub_pixel_sensor() << "m\n";
	
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		tab.str()
	);

	return out.str();
}
//------------------------------------------------------------------------------
std::string Geometry::get_sub_pixel_print()const{

	std::stringstream out;
	out << "Sub_Pixel__\n";

	std::stringstream tab;
	tab << "per pixel........... " << sub_pixel_per_pixel() << "\n";
	tab << "total number........ " << sub_pixel_grid.size() << "\n";
	tab << "outer radius........ " << sub_pixel_outer_radius() << "m\n";
	tab << "inner radius........ " << sub_pixel_inner_radius() << "m\n";
	tab << "spacing............. " << sub_pixel_spacing() << "m\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		tab.str()
	);

	return out.str();
}
//------------------------------------------------------------------------------
std::string Geometry::get_concentrator_bin_print()const{

	std::stringstream out;
	out << "Concentrator_Bin__\n";

	std::stringstream tab;
	tab << "bin_hight........... " << bin_hight() << "m\n";
	tab << "reflectivity........ same as reflector mirrors\n";

	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"  ", 
		tab.str()
	);

	return out.str();
}
//------------------------------------------------------------------------------
double Geometry::principal_aperture_radius_to_throw_photons_in()const {
	return 1.05*(
		reflector.max_outer_aperture_radius() + 
		reflector.thickness_of_dish()*tan(
			max_FoV_radius()
		)
	);
}
//------------------------------------------------------------------------------
double Geometry::field_of_view_solid_angle()const {
	return get_solid_angle_for_opening_angle(max_FoV_radius());
}
//------------------------------------------------------------------------------
} //lightfield