#include "PlenopticIactConfig.h"
#include <math.h> 
#include "HexGridXy.h"
//------------------------------------------------------------------------------
PlenopticIactConfig::PlenopticIactConfig() {

	// main reflector
	focal_length = 70;
	max_aperture_diameter = 50.0;
	mirror_facet_inner_hex_radius = 0.60;
	mirror_facet_cell_overhead = 1.01;
	//mirror_facet_reflecivity_vs_wavelength = &Function::Constant::void_function;

	//image sensor
	field_of_view = Deg2Rad(6.5);
	field_of_view_per_pixel_flat_to_flat = Deg2Rad(0.1);
	pixels_sensor_radius_overhead = 0.9;
	//Func1D* pixel_lens_refractivity_vs_wavelength;		

	init_pending_fields();
}
//------------------------------------------------------------------------------
void PlenopticIactConfig::init_pending_fields() {

	HexGridXy gridgen(max_image_sensor_radius(), 2.0*pixel_outer_hex_radius());
	pixel_grid = gridgen.get_grid();
}
//------------------------------------------------------------------------------
// Reflector
//------------------------------------------------------------------------------
double PlenopticIactConfig::min_FNumber()const {
	return focal_length/max_aperture_diameter;
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::max_aperture_radius()const {
	return 0.5*max_aperture_diameter;
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::naive_aperture_area()const {
	return max_aperture_radius()*max_aperture_radius()*M_PI;
}
//------------------------------------------------------------------------------
// Pixel
//------------------------------------------------------------------------------
double PlenopticIactConfig::pixel_outer_hex_radius()const {
	return pixel_inner_hex_radius()*outer_over_inner_hex_radius;
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::pixel_inner_hex_radius()const {
	return focal_length*tan(field_of_view_per_pixel_flat_to_flat/2.0);
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::pixels_sensor_radius()const {
	return pixel_inner_hex_radius()*pixels_sensor_radius_overhead;
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::pixels_micro_lens_focal_length()const {
	// taking the off axis position of the most outer pixel on the image
	// sensor into account
	return (pixels_sensor_radius()*focal_length)/
		( max_aperture_radius() + max_image_sensor_radius() );
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::pixels_micro_lens_min_FNumber()const {
	return pixels_micro_lens_focal_length()/(2.0*pixel_outer_hex_radius());
}
//------------------------------------------------------------------------------
// Image sensor
//------------------------------------------------------------------------------
double PlenopticIactConfig::max_image_sensor_radius()const {
	return focal_length*tan(field_of_view/2.0);
}
//------------------------------------------------------------------------------
double PlenopticIactConfig::image_sensor_area()const {
	return max_image_sensor_radius()*max_image_sensor_radius()*M_PI;
}
//------------------------------------------------------------------------------
std::string PlenopticIactConfig::get_print()const {

	std::stringstream out;
	//out.setf(std::ios_base::fixed, std::ios_base::floatfield);
	out << std::setprecision(4);
	out << "__Plenoptic_IACT_config__\n";
	out << "\n";
	out << " __Segmented_Reflector__\n";
	out << "  focal length................... " << focal_length << "m\n";
	out << "  max aperture diameter.......... " << 2.0*max_aperture_radius() << "m\n";
	out << "  F-Number min................... " << min_FNumber() << "\n";
	out << "  naive area..................... " << naive_aperture_area() << "m^2\n";
	out << "\n";
	out << " __hex_mirror_facets__\n";
	out << "  inner hex radius............... " << mirror_facet_inner_hex_radius <<"\n";
	out << "  area........................... " << mirror_facet_inner_hex_radius <<"\n";
	out << "  facet cell overhead............ " << mirror_facet_cell_overhead <<"\n";
	out << "\n";
	out << " __Image_sensor__\n";
	out << "  fov............................ " << Rad2Deg(field_of_view) << "Deg\n";
	out << "  max diameter................... " << 2.0*max_image_sensor_radius() << "m\n";
	out << "  number of pixels............... " << pixel_grid.size() << "\n";
	out << "  area........................... " << image_sensor_area() << "m^2\n";
	out << "\n";
	out << " _Pixel_\n";        
	out << "  fov w.r.t main aperture........ " << Rad2Deg(field_of_view_per_pixel_flat_to_flat) << "Deg\n";
	out << "  outer hex diameter............. " << 2.0*pixel_outer_hex_radius()*1e3 << "mm\n";
	out << "  inner hex diameter............. " << 2.0*pixel_inner_hex_radius()*1e3 << "mm\n";
	out << "  sensor overhead................ " << pixels_sensor_radius_overhead << "\n";
	out << "  sensor diameter................ " << 2.0*pixels_sensor_radius()*1e3 << "mm\n";
	out << "  micro lens focal length........ " << pixels_micro_lens_focal_length()*1e3 << "mm\n";
	out << "  micro lens F-Number............ " << pixels_micro_lens_min_FNumber() << "\n";
	out << "\n";
	
	return out.str();
}
//------------------------------------------------------------------------------
