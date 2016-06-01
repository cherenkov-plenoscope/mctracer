#include "gtest/gtest.h"
//#include "Cameras/FlyingCamera.h"
#include "Core/Function/ConstantFunction.h"
#include "Core/Photons.h"
#include "Core/PhotonAndFrame.h"
#include "Geometry/BiConvexLens.h"
#include "Geometry/Disc.h"
#include "Geometry/BiConvexLensHexBound.h"
#include "LensMaker/LensMaker.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Tools/AsciiIo.h"

class BiConvexLensTest : public ::testing::Test {
protected:

	Frame* test_bench;
	TracerSettings settings;
	Random::Mt19937 prng;
	PropagationEnvironment lens_test_bench_environment;
	PhotonSensors::Sensors sensor_list;
	PhotonSensor::Sensor *sensor;
	LensMaker::Config cfg;

	//------------------
	BiConvexLensTest() {
		set_up_settings();
		set_up_test_bench();

		prng.set_seed(Random::zero_seed);

		lens_test_bench_environment.world_geometry = test_bench;
		lens_test_bench_environment.propagation_options = &settings;
		lens_test_bench_environment.random_engine = &prng;
	}
	//------------------
	void set_up_settings() {
		settings.max_number_of_interactions_per_photon = 5;   
	}
	//------------------
	void set_up_test_bench() {
		BiConvexLens* lens = get_preassambled_lens();
		Disc* image_sensor = get_preassambled_sensor_disc();
		test_bench = get_test_bench_frame();

		test_bench->set_mother_and_child(lens);
		test_bench->set_mother_and_child(image_sensor);

		test_bench->init_tree_based_on_mother_child_relations();     
	}
	//------------------  
	BiConvexLens* get_preassambled_lens() {
		
		cfg.focal_length = 1.0;
		cfg.aperture_radius = 0.125;
		cfg.refractive_index = 1.49;

		BiConvexLens* lens;
		lens = new BiConvexLens;

		lens->set_name_pos_rot(
			"little_lens",
			Vec3::null,
			Rot3::null
		);

		Color* lens_color;
		lens_color = new Color(255, 128, 128);

		Function::Limits limits(200e-9, 1200e-9);
		Function::Constant* refraction_vs_wavelength = 
			new Function::Constant(1.49, limits);

		//////////////////////////////////
		lens->set_outer_color(lens_color);
		lens->set_inner_color(lens_color);
		lens->set_inner_refraction(refraction_vs_wavelength);
		lens->set_curvature_radius_and_aperture_radius(
			LensMaker::Approximation::get_curvature_radius(cfg),
			cfg.aperture_radius
		);
		return lens;
	}
	//------------------
	Disc* get_preassambled_sensor_disc() {

		Disc* image_sensor;
		image_sensor = new Disc;

		image_sensor->set_name_pos_rot(
			"sensor_disc",
			Vec3(0.0, 0.0, -1.0), 
			Rot3::null
		);

		Color* sensor_color;
		sensor_color = new Color(128, 255, 128);

		image_sensor->set_outer_color(sensor_color);
		image_sensor->set_inner_color(sensor_color);
		image_sensor->set_radius(cfg.aperture_radius); 

		sensor = new PhotonSensor::Sensor(0, image_sensor);
		std::vector<PhotonSensor::Sensor*> sensors_vector = {sensor};
		sensor_list.init(sensors_vector);

		return image_sensor;   
	}
	//------------------
	Frame* get_test_bench_frame() {

		Frame* test_bench;
		test_bench = new Frame;

		test_bench->set_name_pos_rot(
			"BiConvexLens_test_world",
			Vec3::null,
			Rot3::null
		);  

		
		return test_bench;
	}
	//------------------
	virtual ~BiConvexLensTest() {
	//------------------
	}
};
//----------------------------------------------------------------------
TEST_F(BiConvexLensTest, send_photon_frontal_into_lens) {

	uint total_propagations = 1e4; 
	uint number_of_photons_reaching_sensor_disc = 0;
	
	for(uint i=0; i<total_propagations; i++) {

		Photon blue_photon(Vec3(0.0, 0.0, 1.0), Vec3(0.0, 0.0, -1.0), 433e-9);
		PhotonAndFrame::Propagator(&blue_photon, lens_test_bench_environment);
		//blue_photon.propagate_in(lens_test_bench_environment);

		if(2.0 == blue_photon.get_accumulative_distance())
			number_of_photons_reaching_sensor_disc++;
	}

	EXPECT_NEAR(
		0.97,
		double(number_of_photons_reaching_sensor_disc)/double(total_propagations),
		5.0e-2
	);
}
//----------------------------------------------------------------------
TEST_F(BiConvexLensTest, send_photons_frontal_into_lens_with_offset) {

	// light source
	uint number_of_photons_emitted = 1e4;
    std::vector<Photon*>* photons = 
	    Photons::Source::parallel_towards_z_from_xy_disc(0.125, number_of_photons_emitted);

	HomTra3 Trafo;
	Trafo.set_transformation(
		Rot3(0.0,-Deg2Rad(180.0),0.0), 
		Vec3(0.0, 0.0 ,1.0)
	);

	Photons::transform_all_photons(Trafo, photons);

	// photon propagation
	Photons::propagate_photons_in_world_with_settings(
		photons, 
		lens_test_bench_environment.world_geometry, 
		lens_test_bench_environment.propagation_options,
		&prng
	);	

	// detect photons in sensors
	sensor_list.clear_history();
	sensor_list.assign_photons(photons);

	Photons::delete_photons(photons);

	EXPECT_NEAR(1.5e-3, sensor->point_spread_std_dev(), 1e-3);

	EXPECT_NEAR(
		1.0, 
		double(sensor->get_arrival_table().size())/double(number_of_photons_emitted), 
		10e-2
	);

	/*FlyingCamera free(
		lens_test_bench_environment.world_geometry, 
		lens_test_bench_environment.propagation_options
	);*/
}