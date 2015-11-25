#include "LightFieldTelescope.h"
#include "HexGridAnnulus.h"
#include "HexPlane.h"
#include "Annulus.h"
#include "Disc.h"
#include "Cylinder.h"
#include "BiConvexLensHexBound.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Factory::Factory(const Config ncfg): geometry(ncfg), cfg(ncfg) {

}
//------------------------------------------------------------------------------
Frame* Factory::get_lens_with_name_at_pos(
	const std::string name, 
	const Vector3D pos
) {

	BiConvexLensHexBound* lens = new BiConvexLensHexBound;
	lens->set_name_pos_rot(name, pos, Rotation3D::null);
	lens->set_outer_color(&Color::white);
	lens->set_inner_color(&Color::white);
	lens->set_inner_refraction(cfg.lens_refraction);
	lens->set_curvature_radius_and_outer_hex_radius(
		geometry.pixel_lens_curvature_radius(),
		geometry.pixel_lens_outer_aperture_radius()
	);

	return lens;
}
//------------------------------------------------------------------------------
Frame* Factory::get_lens_array() {

	Frame* lens_array = new Frame(
		"lens_array",
		Vector3D::null,
		Rotation3D::null
	);

	std::vector<Vector3D> pixel_positions = geometry.pixel_positions();
	 
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
Frame* Factory::get_image_sensor_faceplate() {

	HexGridAnnulus face_plate_grid(
		geometry.max_outer_sensor_radius() + 
		geometry.pixel_spacing(),
		geometry.max_outer_sensor_radius() - 
		geometry.pixel_lens_outer_aperture_radius(),
		geometry.pixel_spacing()
	);

	std::vector<Vector3D> face_plate_positions = face_plate_grid.get_grid();

	Frame* face_plate = new Frame(
		"face_plate", 
		Vector3D::null, 
		Rotation3D::null
	);

	for(uint i=0; i<face_plate_positions.size(); i++) {

		HexPlane* face = new HexPlane;
		face->set_name_pos_rot(
			"face_"+std::to_string(i),
			face_plate_positions.at(i),
			Rotation3D::null
		);
		face->set_outer_color(&Color::gray);
		face->set_inner_color(&Color::gray);
		face->set_outer_hex_radius(geometry.pixel_lens_outer_aperture_radius());

		face_plate->set_mother_and_child(face);
	}

	Annulus* outer_front_ring = new Annulus;
	outer_front_ring->set_name_pos_rot(
		"outer_front_ring",
		Vector3D::null, 
		Rotation3D::null
	);
	outer_front_ring->set_outer_color(&Color::gray);
	outer_front_ring->set_inner_color(&Color::gray);
	outer_front_ring->set_outer_inner_radius(
		geometry.outer_sensor_housing_radius(),
		geometry.max_outer_sensor_radius()
	);
	face_plate->set_mother_and_child(outer_front_ring);

	face_plate->cluster_using_helper_frames();
	return face_plate;
}
//------------------------------------------------------------------------------
Frame* Factory::get_sub_pixel_sensor_plane() {

	Frame* sub_pixel_array = new Frame(
		"sub_pixel_array", 
		Vector3D(0.0, 0.0, geometry.pixel_lens_focal_length()), 
		Rotation3D::null
	);

	std::vector<Vector3D> sub_pixel_positions = geometry.sub_pixel_positions();
	 
	for(uint i=0; i<sub_pixel_positions.size(); i++) {

		Frame* subpix = get_sub_pixel_with_name_pos(
			"subpix_" + std::to_string(i),
			sub_pixel_positions.at(i)
		);

		sub_pixel_array->set_mother_and_child(subpix);

		PhotonSensor::PerfectSensor* sub_pixel_sensor = 
			new PhotonSensor::PerfectSensor(i, subpix);

		sub_pixel_sensors.push_back(sub_pixel_sensor);
	}

	sub_pixel_array->cluster_using_helper_frames();
	return sub_pixel_array;
}
//------------------------------------------------------------------------------
Frame* Factory::get_sub_pixel_with_name_pos(
	const std::string name, 
	const Vector3D pos
) {

	HexPlane* subpix = new HexPlane;
	subpix->set_name_pos_rot(name, pos, Rotation3D::null);
	subpix->set_outer_color(&Color::red);
	subpix->set_inner_color(&Color::red);
	subpix->set_outer_hex_radius(geometry.sub_pixel_outer_radius());

	return subpix;
}
//------------------------------------------------------------------------------
Frame* Factory::get_image_sensor_housing()const {

	double housing_height =  2.0*geometry.outer_sensor_housing_radius();

	Frame* sensor_housing = new Frame(
		"sensor_housing", 
		Vector3D(0.0, 0.0, 0.0), 
		Rotation3D::null
	);

	Disc* sensor_housing_top = new Disc;
	sensor_housing_top->set_name_pos_rot(
		"sensor_housing_top",
		Vector3D(0.0, 0.0, housing_height),
		Rotation3D::null
	);
	sensor_housing_top->set_outer_color(&Color::gray);
	sensor_housing_top->set_inner_color(&Color::gray);
	sensor_housing_top->set_radius(geometry.outer_sensor_housing_radius());

	Cylinder* sensor_housing_cylinder = new Cylinder;
	sensor_housing_cylinder->set_name_pos_rot(
		"sensor_housing_cylinder",
		Vector3D::null,
		Rotation3D::null
	);
	sensor_housing_cylinder->set_outer_color(&Color::gray);
	sensor_housing_cylinder->set_inner_color(&Color::gray);
	sensor_housing_cylinder->set_cylinder(
		geometry.outer_sensor_housing_radius(),
		Vector3D(0.0, 0.0, 0.0), 
		Vector3D(0.0, 0.0, housing_height)
	);

	sensor_housing->set_mother_and_child(sensor_housing_cylinder);
	sensor_housing->set_mother_and_child(sensor_housing_top);
	return sensor_housing;
}
//------------------------------------------------------------------------------
void Factory::add_telescope_to_frame(Frame *frame) {

	Frame* image_sensor_front = new Frame(
		"image_sensor_front",
		Vector3D::null, 
		Rotation3D::null
	);

	Frame* image_sensor = new Frame(
		"image_sensor",
		geometry.reflector.focal_point(), 
		Rotation3D::null
	);

	image_sensor_front->set_mother_and_child(get_lens_array());
	image_sensor_front->set_mother_and_child(get_image_sensor_faceplate());

	image_sensor->set_mother_and_child(get_image_sensor_housing());
	image_sensor->set_mother_and_child(get_sub_pixel_sensor_plane());
	image_sensor->set_mother_and_child(image_sensor_front);

	SegmentedReflector::Factory refl_factory(cfg.reflector);

	frame->set_mother_and_child(image_sensor);
	frame->set_mother_and_child(refl_factory.get_reflector());
	frame->init_tree_based_on_mother_child_relations();
}
//------------------------------------------------------------------------------
std::vector<PhotonSensor::Sensor*> Factory::get_sensors()const {
	return sub_pixel_sensors;
}

} //lightfield