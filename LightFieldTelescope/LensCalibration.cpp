#include "LightFieldTelescope/LensCalibration.h"
#include "Tools/AsciiIo.h"
//#include "Cameras/FreeOrbitCamera.h"
#include "Geometry/HexPlane.h"
#include "Geometry/HexGridAnnulus.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
LensCalibration::LensCalibration(const Geometry *geo):  
	telescope_geometry(geo) 
{
	set_up_test_bench();
	read_out_l.clear_history();

	Vec3 incident_direction(
		telescope_geometry->reflector.max_outer_aperture_radius() + telescope_geometry->max_outer_sensor_radius(),
	 	0.0,
	 	telescope_geometry->reflector.focal_length()
	 );
	uint number_of_photons = 1e5;

	for(uint i=0; i<number_of_photons; i++) {

		Vec3 pos_on_lens_aperture = 
			prng.get_point_on_xy_disc_within_radius(
				telescope_geometry->pixel_lens_outer_aperture_radius()
			);

		Ray back_running_ray(
			pos_on_lens_aperture, 
			incident_direction
		);

		Vec3 support_of_photon = back_running_ray.PositionOnRay(
			telescope_geometry->pixel_lens_outer_aperture_radius()*100.0
		);

		Photon* ph = new Photon(
			support_of_photon,
			back_running_ray.Direction()*-1.0,
			433e-9
		);

		ph->propagate_in(test_bench_environment);
		read_out_l.assign_photon(ph);

		ph->delete_history();
		delete ph;		
	}

	std::stringstream header;
	header << "Lens frontal PSF\n";
	header << "number of emitted photons: " << number_of_photons << "\n";
	header << "focal length: " << telescope_geometry->pixel_lens_focal_length() << "m\n";
	header << "curvature radius: " << telescope_geometry->pixel_lens_curvature_radius() << "m\n";
	header << "sensor frame name: " << read_out->get_frame()->get_name() << "\n";

	std::string head;
	head += header.str();
	head += read_out->get_arrival_table_header();

	//FlyingCamera cam(&test_bench, &settings);

	AsciiIo::write_table_to_file_with_header(
		read_out->get_arrival_table(),
		"the_big_lebowsky_lens.csv",
		head
	);
}
//------------------------------------------------------------------------------
LensCalibration::~LensCalibration() {
	delete read_out;
}
//------------------------------------------------------------------------------
void LensCalibration::set_up_test_bench() {

	
	test_bench.set_name_pos_rot("test_bench", Vec3::null, Rot3::null);

	lens.set_name_pos_rot("lens", Vec3::null, Rot3::null);
	lens.set_outer_color(&Color::white);
	lens.set_inner_color(&Color::white);
	lens.set_inner_refraction(telescope_geometry->config.lens_refraction);
	lens.set_curvature_radius_and_outer_hex_radius(
		telescope_geometry->pixel_lens_curvature_radius(),
		telescope_geometry->pixel_lens_outer_aperture_radius()
	);

	// face plate 
	HexGridAnnulus face_plate_grid(
		10.0*telescope_geometry->pixel_lens_outer_aperture_radius(),
		0.5*telescope_geometry->pixel_lens_outer_aperture_radius(),
		telescope_geometry->pixel_spacing()
	);
	std::vector<Vec3> face_plate_positions = face_plate_grid.get_grid();

	face_plate.set_name_pos_rot("face_plate", Vec3::null, Rot3::null);	

	for(uint i=0; i<face_plate_positions.size(); i++) {

		HexPlane* face = new HexPlane;
		face->set_name_pos_rot(
			"face_"+std::to_string(i),
			face_plate_positions.at(i),
			Rot3::null
		);
		face->set_outer_color(&Color::gray);
		face->set_inner_color(&Color::gray);
		face->set_outer_hex_radius(telescope_geometry->
			pixel_lens_outer_aperture_radius());
		face_plate.set_mother_and_child(face);
	}

	Vec3 sensor_position = Vec3(
		0.0,
		0.0,
		-telescope_geometry->pixel_lens_sub_pixel_distance()
	);

	// bin
	Factory fab(telescope_geometry);
	Frame* bin = fab.get_pixel_bin_with_name_at_pos(
		"single_bin", 
		Vec3(
			0.0,
			0.0, 
			-telescope_geometry->pixel_lens_sub_pixel_distance()+
			telescope_geometry->bin_hight()
		)
	);

	image_sensor.set_name_pos_rot("image_sensor", sensor_position, Rot3::null);
	image_sensor.set_outer_color(&Color::red);
	image_sensor.set_inner_color(&Color::red);
	image_sensor.set_radius(
		5.0*telescope_geometry->pixel_lens_outer_aperture_radius()
	);

	test_bench.set_mother_and_child(&lens);
	test_bench.set_mother_and_child(&face_plate);
	test_bench.set_mother_and_child(&image_sensor);	
	test_bench.set_mother_and_child(bin);

	test_bench.init_tree_based_on_mother_child_relations();
	uint id = 0;
	read_out = new PhotonSensor::Sensor(id, &image_sensor);

	std::vector<PhotonSensor::Sensor*> read_out_list = {read_out};
	read_out_l.init(read_out_list);

	test_bench_environment.world_geometry = &test_bench;
	test_bench_environment.propagation_options = &settings;
	test_bench_environment.random_engine = &prng;
}
//------------------------------------------------------------------------------
std::string LensCalibration::get_print()const {

	return "LensCalibration\n";
}
//------------------------------------------------------------------------------
} // namespace LightFieldTelescope