#include "Plenoscope/LightFieldSensor/Factory.h"
#include "Geometry/HexGridAnnulus.h"
#include "Geometry/HexPlane.h"
#include "Geometry/Annulus.h"
#include "Geometry/Disc.h"
#include "Geometry/Plane.h"
#include "Geometry/Cylinder.h"
#include "Geometry/BiConvexLensHexBound.h"
#include "Geometry/PlaneDualSphericalBound.h"

namespace Plenoscope {
namespace LightFieldSensor {
//------------------------------------------------------------------------------
Factory::Factory(const Geometry* geo): geometry(geo) {}
//------------------------------------------------------------------------------
Frame* Factory::get_lens_with_name_at_pos(
	const std::string name, 
	const Vec3 pos
) {
	BiConvexLensHexBound* lens = new BiConvexLensHexBound;
	lens->set_name_pos_rot(name, pos, Rot3::null);
	lens->set_outer_color(&Color::white);
	lens->set_inner_color(&Color::white);
	lens->set_inner_refraction(geometry->config.lens_refraction);
	lens->set_curvature_radius_and_outer_hex_radius(
		geometry->pixel_lens_curvature_radius(),
		geometry->pixel_lens_outer_aperture_radius()
	);

	return lens;
}
//------------------------------------------------------------------------------
Frame* Factory::get_lens_array() {

	Frame* lens_array = new Frame(
		"lens_array",
		Vec3::null,
		Rot3::null
	);

	std::vector<Vec3> pixel_positions = geometry->pixel_positions();
	 
	for(uint i=0; i<pixel_positions.size(); i++) {

		Frame* lens = get_lens_with_name_at_pos(
			"lens_" + std::to_string(i),
			pixel_positions.at(i)
		);

		lens_array->set_mother_and_child(lens);
	}

	lens_array->cluster_using_helper_frames();
	return lens_array;
}
//------------------------------------------------------------------------------
Frame* Factory::get_pixel_bin_array() {
	
	Frame* bin_array = new Frame(
		"bin_array",
		Vec3(0.0, 0.0, geometry->pixel_lens_sub_pixel_distance()),
		Rot3::null
	);

	std::vector<Vec3> flower_positions = 
		geometry->sub_pixel_flower_positions();

	for(uint i=0; i<flower_positions.size(); i++) {

		Frame* lens = get_pixel_bin_with_name_at_pos(
			"bin_" + std::to_string(i),
			flower_positions.at(i)
		);

		bin_array->set_mother_and_child(lens);
	}

	bin_array->cluster_using_helper_frames();
	return bin_array;
}
//------------------------------------------------------------------------------
Frame* Factory::get_pixel_bin_with_name_at_pos(
	const std::string name, 
	const Vec3 pos
) {
	Frame* bin = new Frame(
		name,
		pos,
		Rot3::null
	);

	double R = geometry->pixel_lens_inner_aperture_radius();
	double hight = geometry->bin_hight();

	for(uint i=0; i<6; i++) {

		double phi = double(i)*1.0/3.0*M_PI;
		
		Plane* binwall = new Plane(
			name + "_" + std::to_string(i),
			Vec3(R*sin(phi), R*cos(phi), -0.5*hight),
			Rot3(M_PI*0.5, 0.0, phi)
		);

		binwall->set_x_y_width(
			geometry->pixel_lens_outer_aperture_radius(),
			hight
		);

		binwall->set_outer_color(&Color::green);
		binwall->set_inner_color(&Color::green);
		binwall->set_outer_reflection(geometry->config.bin_relection);
	
		bin->set_mother_and_child(binwall);
	}

	return bin;
}
//------------------------------------------------------------------------------
Frame* Factory::get_light_field_sensor_frontplate() {

	HexGridAnnulus face_plate_grid(
		geometry->max_outer_sensor_radius() + 
		geometry->pixel_spacing(),
		geometry->max_outer_sensor_radius() - 
		geometry->pixel_lens_outer_aperture_radius(),
		geometry->pixel_spacing()
	);

	std::vector<Vec3> face_plate_positions = face_plate_grid.get_grid();

	Frame* face_plate = new Frame(
		"face_plate", 
		Vec3::null, 
		Rot3::null
	);

	for(uint i=0; i<face_plate_positions.size(); i++) {

		HexPlane* face = new HexPlane;
		face->set_name_pos_rot(
			"face_"+std::to_string(i),
			face_plate_positions.at(i),
			Rot3::null
		);
		face->set_outer_color(&Color::gray);
		face->set_inner_color(&Color::gray);
		face->set_outer_hex_radius(geometry->pixel_lens_outer_aperture_radius());

		face_plate->set_mother_and_child(face);
	}

	Annulus* outer_front_ring = new Annulus;
	outer_front_ring->set_name_pos_rot(
		"outer_front_ring",
		Vec3::null, 
		Rot3::null
	);
	outer_front_ring->set_outer_color(&Color::gray);
	outer_front_ring->set_inner_color(&Color::gray);
	outer_front_ring->set_outer_inner_radius(
		geometry->outer_sensor_housing_radius(),
		geometry->max_outer_sensor_radius()
	);
	face_plate->set_mother_and_child(outer_front_ring);

	face_plate->cluster_using_helper_frames();
	return face_plate;
}
//------------------------------------------------------------------------------
Frame* Factory::get_sub_pixel_sensor_plane() {

	Frame* sub_pixel_array = new Frame(
		"sub_pixel_array", 
		Vec3(0.0, 0.0, geometry->pixel_lens_sub_pixel_distance()), 
		Rot3::null
	);

	const std::vector<Vec3> &sub_pixel_positions = geometry->sub_pixel_positions();

	std::vector<PhotonSensor::Sensor*> sub_pixels;
	sub_pixels.reserve(sub_pixel_positions.size());

	for(uint i=0; i<sub_pixel_positions.size(); i++) {

		Frame* subpix = get_sub_pixel_with_name_pos(
			"subpix_" + std::to_string(i),
			sub_pixel_positions.at(i)
		);

		sub_pixel_array->set_mother_and_child(subpix);

		PhotonSensor::Sensor* sub_pixel_sensor = 
			new PhotonSensor::Sensor(i, subpix);

		sub_pixels.push_back(sub_pixel_sensor);
	}

	sub_pixel_array->cluster_using_helper_frames();

	sub_pixel_sensors = new PhotonSensors::Sensors(sub_pixels);
	return sub_pixel_array;
}
//------------------------------------------------------------------------------
Frame* Factory::get_sub_pixel_with_name_pos(
	const std::string name, 
	const Vec3 pos
) {

	HexPlane* subpix = new HexPlane;
	subpix->set_name_pos_rot(
		name, 
		pos, 
		Rot3(0.0, 0.0, geometry->sub_pixel_z_orientation())
	);
	subpix->set_outer_color(&Color::red);
	subpix->set_inner_color(&Color::red);
	subpix->set_outer_hex_radius(geometry->sub_pixel_outer_radius());

	return subpix;
}
//------------------------------------------------------------------------------
Frame* Factory::get_image_sensor_housing()const {

	double housing_height =  2.0*geometry->outer_sensor_housing_radius();

	Frame* sensor_housing = new Frame(
		"sensor_housing", 
		Vec3(0.0, 0.0, 0.0), 
		Rot3::null
	);

	Disc* sensor_housing_top = new Disc;
	sensor_housing_top->set_name_pos_rot(
		"top",
		Vec3(0.0, 0.0, housing_height),
		Rot3::null
	);
	sensor_housing_top->set_outer_color(&Color::gray);
	sensor_housing_top->set_inner_color(&Color::gray);
	sensor_housing_top->set_radius(geometry->outer_sensor_housing_radius());

	Cylinder* sensor_housing_cylinder = new Cylinder(
		"cylinder",
		Vec3::null,
		Rot3::null
	);
	sensor_housing_cylinder->set_outer_color(&Color::gray);
	sensor_housing_cylinder->set_inner_color(&Color::gray);
	sensor_housing_cylinder->set_cylinder(
		geometry->outer_sensor_housing_radius(),
		Vec3(0.0, 0.0, 0.0), 
		Vec3(0.0, 0.0, housing_height)
	);

	sensor_housing->set_mother_and_child(sensor_housing_cylinder);
	sensor_housing->set_mother_and_child(sensor_housing_top);
	return sensor_housing;
}
//------------------------------------------------------------------------------
void Factory::add_light_field_sensor_to_frame(Frame *frame) {
	
	Frame* light_field_sensor_front = new Frame(
		"front",
		Vec3::null, 
		Rot3::null
	);

	light_field_sensor_front->set_mother_and_child(get_lens_array());
	light_field_sensor_front->set_mother_and_child(get_light_field_sensor_frontplate());

	frame->set_mother_and_child(get_image_sensor_housing());
	frame->set_mother_and_child(get_pixel_bin_array());
	frame->set_mother_and_child(get_sub_pixel_sensor_plane());
	frame->set_mother_and_child(light_field_sensor_front);
}
//------------------------------------------------------------------------------
PhotonSensors::Sensors* Factory::get_sub_pixels()const {
	return sub_pixel_sensors;
}
//------------------------------------------------------------------------------
}//LightFieldSensor
}//Plenoscope