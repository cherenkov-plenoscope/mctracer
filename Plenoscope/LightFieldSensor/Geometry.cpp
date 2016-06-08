#include "Plenoscope/LightFieldSensor/Geometry.h"
#include "Geometry/HexGridAnnulus.h"
#include "Geometry/GridNeighborhoodTopoligy.h"
#include "Tools/AsciiIo.h"
#include "Tools/StringTools.h"
#include "Geometry/HexGridFlower.h"
#include "LensMaker/LensMaker.h"

namespace Plenoscope {
namespace LightFieldSensor {
//------------------------------------------------------------------------------
Geometry::Geometry(const Config ncfg): 
	config(ncfg)
{	
	set_up_pixel_grid();
	set_up_paxel_grid_centers_of_pixels();
	set_up_paxel_per_pixel_template_grid();
	set_up_lixel_grid();
	set_up_pixel_lens_geometry();
	std::cout << get_print();
}
//------------------------------------------------------------------------------
void Geometry::set_up_pixel_grid() {

	HexGridAnnulus pixgrid(
		max_outer_sensor_radius() - pixel_lens_outer_aperture_radius(), 
		pixel_spacing()
	);

	pixel_grid = pixgrid.get_grid();

	GridNeighborhoodTopoligy topology(
		&pixel_grid, 
		1.1*pixel_spacing()
	);
	pixel_neighborhood = topology.get_neighbor_relations();
}
//------------------------------------------------------------------------------
void Geometry::set_up_paxel_per_pixel_template_grid() {

	HexGridFlower paxel_template_grid_generator(
		pixel_lens_outer_aperture_radius(),
		config.number_of_paxel_on_pixel_diagonal
	);

	lixel_hex_flat2flat_diameter = paxel_template_grid_generator.get_facet_spacing();
	paxel_per_pixel_template_grid = paxel_template_grid_generator.get_grid();

	GridNeighborhoodTopoligy topology(
		&paxel_per_pixel_template_grid, 
		1.1*lixel_hex_flat2flat_diameter
	);
	paxel_neighborhood = topology.get_neighbor_relations();
}
//------------------------------------------------------------------------------
void Geometry::set_up_paxel_grid_centers_of_pixels() {

	paxel_grid_centers_of_pixels.reserve(pixel_grid.size());

	const double r = 1.0 + pixel_plane_to_paxel_plane_distance()/
		config.expected_imaging_system_focal_length;

	for(Vec3 pixel_pos : pixel_grid)
		paxel_grid_centers_of_pixels.push_back(
			pixel_pos*r
		);
}
//------------------------------------------------------------------------------
void Geometry::set_up_lixel_grid() {

	lixel_grid.reserve(
		paxel_grid_centers_of_pixels.size()*
		paxel_per_pixel_template_grid.size()
	);

	for(Vec3 flower_pos : paxel_grid_centers_of_pixels)
		for(Vec3 paxel_pos : paxel_per_pixel_template_grid)
			lixel_grid.push_back(flower_pos + paxel_pos);
}
//------------------------------------------------------------------------------
void Geometry::set_up_pixel_lens_geometry() {
	
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
	return config.expected_imaging_system_focal_length*tan(max_FoV_radius());
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
	return config.expected_imaging_system_focal_length*tan(config.pixel_FoV_hex_flat2flat/2.0);
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_outer_aperture_radius()const {
	return pixel_lens_inner_aperture_radius()*2.0/sqrt(3.0);
}
//------------------------------------------------------------------------------
double Geometry::aperture_image_radius_on_paxel_plane()const {
	return pixel_lens_inner_aperture_radius() - 0.5*lixel_inner_radius();
}
//------------------------------------------------------------------------------
double Geometry::pixel_lens_focal_length()const {
	const double expected_imaging_system_naive_f_over_D = 
		config.expected_imaging_system_focal_length/
			(2.0*config.expected_imaging_system_max_aperture_radius);

	return expected_imaging_system_naive_f_over_D*2.0*aperture_image_radius_on_paxel_plane();
}
//------------------------------------------------------------------------------
double Geometry::pixel_plane_to_paxel_plane_distance()const {
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
std::vector<Vec3> Geometry::pixel_positions()const {
	return pixel_grid;
}
//------------------------------------------------------------------------------
vector<vector<uint>> Geometry::pixel_neighbor_relations()const {
	return pixel_neighborhood;
}
//------------------------------------------------------------------------------
const std::vector<Vec3>& Geometry::lixel_positions()const {
	return lixel_grid;
}
//------------------------------------------------------------------------------
vector<vector<uint>> Geometry::paxel_neighbor_relations()const {
	return paxel_neighborhood;
}
//------------------------------------------------------------------------------
std::vector<Vec3> Geometry::paxel_grid_center_positions()const {
	return paxel_grid_centers_of_pixels;
}
//------------------------------------------------------------------------------
double Geometry::lixel_z_orientation()const {
	return 1.0/6.0*M_PI;
}
//------------------------------------------------------------------------------
double Geometry::number_of_paxel_per_pixel()const {
	return paxel_per_pixel_template_grid.size();
}
//------------------------------------------------------------------------------
double Geometry::lixel_outer_radius()const {
	return lixel_inner_radius()*2.0/sqrt(3.0);
}
//------------------------------------------------------------------------------
double Geometry::lixel_inner_radius()const {
	return 0.5*lixel_spacing();
}
//------------------------------------------------------------------------------
double Geometry::lixel_spacing()const {
	return lixel_hex_flat2flat_diameter;
}
//------------------------------------------------------------------------------
uint Geometry::number_of_lixel()const {
	return lixel_grid.size();
}
//------------------------------------------------------------------------------
double Geometry::bin_hight()const {
	return pixel_plane_to_paxel_plane_distance()*0.25;
}
//------------------------------------------------------------------------------
double Geometry::expected_imaging_system_focal_length()const {
	return config.expected_imaging_system_focal_length;
}
//------------------------------------------------------------------------------
double Geometry::expected_imaging_system_max_aperture_radius()const{
	return config.expected_imaging_system_max_aperture_radius;
}
//------------------------------------------------------------------------------
void Geometry::write_lixel_positions(const string path)const {

	vector<vector<double>> all_lixel_xy;

	for(Vec3 pos : lixel_grid) {
		vector<double> lixel_xy = {pos.x(), pos.y()};
		all_lixel_xy.push_back(lixel_xy);
	}

	stringstream header;
	header << "number_of_lixels = " << lixel_grid.size() << "\n";
	header << "number_of_paxel_per_pixel = " << number_of_paxel_per_pixel() << "\n";
	header << "lixel_z_orientation = " << lixel_z_orientation() << "\n";
	header << "lixel_inner_radius = " << lixel_inner_radius() << "\n";
	header << "lixel_outer_radius = " << lixel_outer_radius() << "\n";
	header << "x[m]\ty[m]\n";

	AsciiIo::write_table_to_file_with_header(
		all_lixel_xy,
		path,
		header.str()
	);
}
//------------------------------------------------------------------------------
string Geometry::get_print()const{
	
	stringstream tab;
	tab << "__Light_Field_Sensor__\n";
	tab << " Field of View................. " << Rad2Deg(config.max_FoV_diameter) << "deg\n";
	tab << " Field of View solid angle..... " << field_of_view_solid_angle() << "sr\n";
	tab << " Max sensor radius............. " << max_outer_sensor_radius() << "m\n";
	tab << " Number of pixels.............. " << number_of_pixels() << "\n";
	tab << " Number of paxels in pixel..... " << number_of_paxel_per_pixel() << "\n";
	tab << " Number of lixel............... " << lixel_grid.size() << "\n";
	tab << "\n";
	tab << "__Pixel_Lens__\n";
	tab << " FoV hex flat2flat....... " << Rad2Deg(pixel_FoV_hex_flat2flat()) << "deg\n";
	tab << " Spacing................. " << pixel_spacing() << "m\n";
	tab << " Inner aperture radius... " << pixel_lens_inner_aperture_radius() << "m\n";
	tab << " Outer aperture radius... " << pixel_lens_outer_aperture_radius() << "m\n";
	tab << " Lens f over D........... " << pixel_lens_f_over_D() << "\n";
	tab << " Lens focal length....... " << pixel_lens_focal_length() << "m\n";
	tab << " Lens curvature radius... " << pixel_lens_curvature_radius() << "m\n";
	tab << " Lens to paxel plane dist " << pixel_plane_to_paxel_plane_distance() << "m\n";
	tab << " Lens refraction mean.... " << pixel_lens_mean_refraction() << "\n";
	tab << " Expected aperture image radius... " << aperture_image_radius_on_paxel_plane() << "m\n";
	tab << "\n";
	tab << "__Lixel__\n";
	tab << " Outer radius........ " << lixel_outer_radius() << "m\n";
	tab << " Inner radius........ " << lixel_inner_radius() << "m\n";
	tab << " Spacing............. " << lixel_spacing() << "m\n";
	tab << "\n";
	tab << "__Concentrator_Bin__\n";
	tab << " Bin_hight........... " << bin_hight() << "m\n";
	tab << " Reflectivity........ " << config.bin_relection->get_mean(137) << "\n";
	return tab.str();		
}
//------------------------------------------------------------------------------
double Geometry::field_of_view_solid_angle()const {
	return get_solid_angle_for_opening_angle(max_FoV_radius());
}
//------------------------------------------------------------------------------
	}//LightFieldSensor
}//Plenoscope