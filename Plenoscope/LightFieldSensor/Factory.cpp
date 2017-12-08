#include "Plenoscope/LightFieldSensor/Factory.h"
#include "Scenery/Geometry/HexGridAnnulus.h"
#include "Scenery/Primitive/HexPlane.h"
#include "Scenery/Primitive/Annulus.h"
#include "Scenery/Primitive/Disc.h"
#include "Scenery/Primitive/Plane.h"
#include "Scenery/Primitive/Cylinder.h"
#include "Scenery/Primitive/BiConvexLensHexBound.h"
#include "Scenery/Primitive/PlaneDualSphericalBound.h"
#include <math.h>
using std::vector;
using std::string;

namespace Plenoscope {
namespace LightFieldSensor {
//------------------------------------------------------------------------------
Factory::Factory(const Geometry* geo): 
	geometry(geo), 
	scenery(&Scenery::void_scenery) 
{}
//------------------------------------------------------------------------------
void Factory::add_lens_array(Frame* frame) {
	
	scenery->colors.add("lens_white", Color::WHITE);
	const Color* white = scenery->colors.get("lens_white");

	Frame* lens_array = frame->append<Frame>();
	lens_array->set_name_pos_rot("lens_array", Vec3::ORIGIN, Rot3::UNITY);
	vector<Vec3> pixel_positions = geometry->pixel_positions(); 

	for(unsigned int i=0; i<pixel_positions.size(); i++) {

		BiConvexLensHexBound* lens = lens_array->append<BiConvexLensHexBound>();
		lens->set_name_pos_rot(
			"lens_" + std::to_string(i), 
			pixel_positions.at(i), 
			Rot3::UNITY
		);
		lens->set_outer_color(white);
		lens->set_inner_color(white);
		lens->set_inner_refraction(geometry->config.lens_refraction);
		lens->set_curvature_radius_and_outer_hex_radius(
			geometry->pixel_lens_curvature_radius(),
			geometry->pixel_lens_outer_aperture_radius()
		);
	}
}
//------------------------------------------------------------------------------
void Factory::add_pixel_bin_array(Frame* frame) {
	
	Frame* bin_array = frame->append<Frame>();
	bin_array->set_name_pos_rot(
		"bin_array",
		Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()),
		Rot3::UNITY
	);

	vector<Vec3> flower_positions = geometry->paxel_grid_center_positions();
	scenery->colors.add("bin_wall_green", Color::GREEN);

	for(unsigned int i=0; i<flower_positions.size(); i++) {
		add_pixel_bin_with_name_at_pos(
			bin_array,
			"bin_" + std::to_string(i),
			flower_positions.at(i)
		);
	}	
}
//------------------------------------------------------------------------------
void Factory::add_pixel_bin_with_name_at_pos(
	Frame* frame,
	const string name, 
	const Vec3 pos
) {
	const Color* green = scenery->colors.get("bin_wall_green");

	Frame* bin = frame->append<Frame>();
	bin->set_name_pos_rot(name,	pos, Rot3::UNITY);

	const double R = geometry->pixel_lens_inner_aperture_radius();
	const double hight = geometry->bin_hight();

	for(unsigned int i=0; i<6; i++) {
		const double phi = double(i)*1.0/3.0*M_PI;
		
		Plane* binwall = bin->append<Plane>();
		binwall->set_name_pos_rot(
			name + "_" + std::to_string(i),
			Vec3(R*sin(phi), R*cos(phi), -0.5*hight),
			Rot3(-M_PI*0.5, 0.0, phi)
		);
		binwall->set_x_y_width(
			geometry->pixel_lens_outer_aperture_radius(),
			hight
		);
		binwall->set_outer_color(green);
		binwall->set_inner_color(green);
		binwall->set_outer_reflection(geometry->config.bin_reflection);
	}
}
//------------------------------------------------------------------------------
void Factory::add_light_field_sensor_frontplate(Frame* frame) {

	scenery->colors.add("light_field_sensor_housing_gray", Color::GRAY);
	const Color* gray = scenery->colors.get("light_field_sensor_housing_gray");

	HexGridAnnulus face_plate_grid(
		geometry->max_outer_sensor_radius() + 
		geometry->pixel_spacing(),
		geometry->max_outer_sensor_radius() - 
		geometry->pixel_lens_outer_aperture_radius(),
		geometry->pixel_spacing()
	);

	vector<Vec3> face_plate_positions = face_plate_grid.get_grid();
	Frame* face_plate = frame->append<Frame>();
	face_plate->set_name_pos_rot("face_plate", Vec3::ORIGIN, Rot3::UNITY);

	for(unsigned int i=0; i<face_plate_positions.size(); i++) {

		HexPlane* face = face_plate->append<HexPlane>();
		face->set_name_pos_rot(
			"face_"+std::to_string(i),
			face_plate_positions.at(i),
			Rot3::UNITY
		);
		face->set_outer_color(gray);
		face->set_inner_color(gray);
		face->set_outer_hex_radius(geometry->pixel_lens_outer_aperture_radius());
	}

	Annulus* outer_front_ring = face_plate->append<Annulus>();
	outer_front_ring->set_name_pos_rot("outer_front_ring", Vec3::ORIGIN, Rot3::UNITY);
	outer_front_ring->set_outer_color(gray);
	outer_front_ring->set_inner_color(gray);
	outer_front_ring->set_outer_inner_radius(
		geometry->outer_sensor_housing_radius(),
		geometry->max_outer_sensor_radius()
	);	
}
//------------------------------------------------------------------------------
void Factory::add_lixel_sensor_plane(Frame* frame) {	

	scenery->colors.add("light_field_sensor_cell_red", Color::RED);
	const Color* red = scenery->colors.get("light_field_sensor_cell_red");

	Frame* sub_pixel_array = frame->append<Frame>();
	sub_pixel_array->set_name_pos_rot(
		"lixel_array", 
		Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()), 
		Rot3::UNITY
	);

	const vector<Vec3> &lixel_positions = geometry->lixel_positions();

	vector<PhotonSensor::Sensor*> sub_pixels;
	sub_pixels.reserve(lixel_positions.size());

	for(unsigned int i=0; i<lixel_positions.size(); i++) {

		HexPlane* subpix = sub_pixel_array->append<HexPlane>(); 
		subpix->set_name_pos_rot(
			"lixel_" + std::to_string(i),
			lixel_positions.at(i),
			Rot3(0.0, 0.0, geometry->lixel_z_orientation())
		);
		subpix->set_outer_color(red);
		subpix->set_inner_color(red);
		subpix->set_outer_hex_radius(geometry->lixel_outer_radius());

		PhotonSensor::Sensor* sub_pixel_sensor = 
			new PhotonSensor::Sensor(i, subpix);

		sub_pixels.push_back(sub_pixel_sensor);
	}

	sub_pixel_sensors = new PhotonSensor::Sensors(sub_pixels);
}
//------------------------------------------------------------------------------
void Factory::add_image_sensor_housing(Frame *frame) {

	double housing_height =  2.0*geometry->outer_sensor_housing_radius();

	Frame* sensor_housing = frame->append<Frame>();
	sensor_housing->set_name_pos_rot("sensor_housing", Vec3::ORIGIN, Rot3::UNITY);

	Disc* sensor_housing_top = sensor_housing->append<Disc>();
	sensor_housing_top->set_name_pos_rot(
		"top",
		Vec3(0.0, 0.0, housing_height),
		Rot3::UNITY
	);
	sensor_housing_top->set_outer_color(&Color::GRAY);
	sensor_housing_top->set_inner_color(&Color::GRAY);
	sensor_housing_top->set_radius(geometry->outer_sensor_housing_radius());

	Cylinder* sensor_housing_cylinder = sensor_housing->append<Cylinder>();
	sensor_housing_cylinder->set_name_pos_rot(
		"cylinder",
		Vec3::ORIGIN,
		Rot3::UNITY
	);
	sensor_housing_cylinder->set_outer_color(&Color::GRAY);
	sensor_housing_cylinder->set_inner_color(&Color::GRAY);
	sensor_housing_cylinder->set_cylinder(
		geometry->outer_sensor_housing_radius(),
		Vec3(0.0, 0.0, 0.0), 
		Vec3(0.0, 0.0, housing_height)
	);
}
//------------------------------------------------------------------------------
void Factory::add_light_field_sensor_to_frame_in_scenery(
	Frame *frame, 
	Scenery* scenery
) {
	this->scenery = scenery;

	Frame* light_field_sensor_front = frame->append<Frame>();
	light_field_sensor_front->set_name_pos_rot(
		"front",
		Vec3::ORIGIN, 
		Rot3::UNITY
	);

	add_lens_array(light_field_sensor_front);
	add_light_field_sensor_frontplate(light_field_sensor_front);

	add_image_sensor_housing(frame);
	add_pixel_bin_array(frame);
	add_lixel_sensor_plane(frame);
}
//------------------------------------------------------------------------------
void Factory::add_demonstration_light_field_sensor_to_frame_in_scenery(
	Frame *frame, 
	Scenery* scenery
) {
	this->scenery = scenery;

	// Add lens
	scenery->colors.add("lens_white", Color::WHITE);
	const Color* white = scenery->colors.get("lens_white");

	BiConvexLensHexBound* lens = frame->append<BiConvexLensHexBound>();
	lens->set_name_pos_rot("lens_0", Vec3(0.0, 0.0, 0.0), Rot3::UNITY);
	lens->set_outer_color(white);
	lens->set_inner_color(white);
	lens->set_inner_refraction(geometry->config.lens_refraction);
	lens->set_curvature_radius_and_outer_hex_radius(
		geometry->pixel_lens_curvature_radius(),
		geometry->pixel_lens_outer_aperture_radius()
	);

	// Add bin walls
	Frame* bin_array = frame->append<Frame>();
	bin_array->set_name_pos_rot(
		"bin_array",
		Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()),
		Rot3::UNITY
	);

	scenery->colors.add("bin_wall_green", Color::GREEN);

	add_pixel_bin_with_name_at_pos(
		bin_array,
		"bin_0",
		Vec3(0.0, 0.0, 0.0)
	);

	// Add lixels
	scenery->colors.add("light_field_sensor_cell_red", Color::RED);
	const Color* red = scenery->colors.get("light_field_sensor_cell_red");

	Frame* sub_pixel_array = frame->append<Frame>();
	sub_pixel_array->set_name_pos_rot(
		"lixel_array", 
		Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()), 
		Rot3::UNITY
	);

	const vector<Vec3> lixel_positions = geometry->paxel_per_pixel_template_grid;

	vector<PhotonSensor::Sensor*> sub_pixels;
	sub_pixels.reserve(geometry->paxel_per_pixel_template_grid.size());

	for(unsigned int i=0; i<lixel_positions.size(); i++) {

		HexPlane* subpix = sub_pixel_array->append<HexPlane>(); 
		subpix->set_name_pos_rot(
			"lixel_" + std::to_string(i),
			lixel_positions.at(i),
			Rot3(0.0, 0.0, geometry->lixel_z_orientation())
		);
		subpix->set_outer_color(red);
		subpix->set_inner_color(red);
		subpix->set_outer_hex_radius(0.95*geometry->lixel_outer_radius());

		PhotonSensor::Sensor* sub_pixel_sensor = 
			new PhotonSensor::Sensor(i, subpix);

		sub_pixels.push_back(sub_pixel_sensor);
	}

	sub_pixel_sensors = new PhotonSensor::Sensors(sub_pixels);
}
//------------------------------------------------------------------------------
PhotonSensor::Sensors* Factory::get_sub_pixels()const {
	return sub_pixel_sensors;
}
//------------------------------------------------------------------------------
}//LightFieldSensor
}//Plenoscope