#include "gtest/gtest.h"

#include <algorithm>

#include "Core/Color.h"
#include "Geometry/BiConvexLensHexBound.h"
#include "LensMaker/LensMaker.h"

class LensMakerTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(LensMakerTest, check_sebastians_paper_and_pen_calculation) {

	double expected_curvature_radius = 0.125;;

	LensMaker::Config cfg;
	cfg.focal_length = 0.1335;
	cfg.aperture_radius = 0.071;
	cfg.refractive_index = 1.49;

	EXPECT_NEAR(
		expected_curvature_radius, 
		LensMaker::Approximation::get_curvature_radius(cfg),
		expected_curvature_radius*3e-2
	);
}
//----------------------------------------------------------------------
TEST_F(LensMakerTest, lens_thicknes) {

	double expected_curvature_radius = 0.125;
	double expected_thickness = 0.0445;

	LensMaker::Config cfg;
	cfg.focal_length = 0.1335;
	cfg.aperture_radius = 0.071;
	cfg.refractive_index = 1.49;

	EXPECT_NEAR(
		expected_thickness, 
		LensMaker::Approximation::get_lens_thickness_for_R_r(
			expected_curvature_radius, 
			cfg.aperture_radius
		),
		expected_thickness*3e-2
	);
}
//----------------------------------------------------------------------
TEST_F(LensMakerTest, check_lensmaker_on_optical_table_with_lens) {

	// Hello LensMaker,
	LensMaker::Config cfg;
	// we want a lens with
	cfg.focal_length = 1.0;//0.1335;
	// made out of pmma plastic with
	cfg.refractive_index = 1.49;
	// and an outer aperture radius of 
	cfg.aperture_radius = 0.071;
	// please tell us the needed curvature radius.

	double lens_curvature_radius = 
		LensMaker::Approximation::get_curvature_radius(cfg);

	// ok lets test it...
	const Color* lens_col = &Color::gray;
	const Color* sensor_disc_col = &Color::dark_gray;

    const Function::Constant* refraction_vs_wavelength = 
    	new Function::Constant(
    		cfg.refractive_index, 
    		Function::Limits(200e-9, 1200e-9)
    	);

    uint number_of_photons_per_run = 1000;
    std::vector<double> sigma_psf_vs_image_sensor_distance;
    std::vector<double> image_sensor_distances;
	for(
		double offset=-cfg.focal_length*0.3;
		offset<cfg.focal_length*0.3;
		offset=offset+cfg.focal_length*0.01
	) {
		double image_sensor_disc_distance = cfg.focal_length + offset;

	    // geometry
		Frame optical_table("table", Vector3D::null, Rotation3D::null);

		BiConvexLensHexBound lens;
		lens.set_name_pos_rot("lens", Vector3D::null, Rotation3D::null);
	    lens.set_outer_color(lens_col);
	    lens.set_inner_color(lens_col);
	    lens.set_inner_refraction(refraction_vs_wavelength);
	    lens.set_curvature_radius_and_outer_hex_radius(
	    	lens_curvature_radius, 
	    	cfg.aperture_radius
	    );

	    Disc sensor_disc;
	    sensor_disc.set_name_pos_rot(
	    	"sensor_disc", 
	    	Vector3D(0.0, 0.0, -image_sensor_disc_distance), 
	    	Rotation3D::null
	    );
	    sensor_disc.set_outer_color(sensor_disc_col);
	    sensor_disc.set_inner_color(sensor_disc_col);
	    sensor_disc.set_radius(cfg.aperture_radius*0.85);
	    PhotonSensor::Xy sensor(0, &sensor_disc);
	    std::vector<PhotonSensor::Sensor*> sensor_vec = {&sensor};
	    PhotonSensors::Sensors sensor_list(sensor_vec);

	    optical_table.set_mother_and_child(&lens);
	    optical_table.set_mother_and_child(&sensor_disc);
	    optical_table.init_tree_based_on_mother_child_relations();

	    // light source
	    std::vector<Photon*>* photons = 
		    Photons::Source::parallel_towards_z_from_xy_disc(
				cfg.aperture_radius*0.85, // 0.85 inner hex radius
				number_of_photons_per_run
			);

		HomoTrafo3D Trafo;
		Trafo.set_transformation(
			Rotation3D(0.0,-Deg2Rad(180.0),0.0), 
			Vector3D(0.0, 0.0 ,2.0)
		);

		Photons::transform_all_photons(Trafo, photons);

		// propagation settings
		TracerSettings settings;	
		settings.SetMultiThread(true);
		settings.store_only_last_intersection(false);

		// photon propagation
		Photons::propagate_photons_in_world_with_settings(
			photons, &optical_table, &settings
		);

		// detect photons in sensors
		sensor_list.clear_history();
		sensor_list.assign_photons(photons);

		sigma_psf_vs_image_sensor_distance.push_back(sensor.point_spread_std_dev());
		image_sensor_distances.push_back(image_sensor_disc_distance);

		Photons::delete_photons_and_history(photons);

		//FreeOrbitCamera free(&optical_table, &settings);
	}

	double min_sigma_psf = *std::min_element(
		sigma_psf_vs_image_sensor_distance.begin(),
		sigma_psf_vs_image_sensor_distance.end()
	);

	uint min_sigma_psf_pos = std::min_element(
			sigma_psf_vs_image_sensor_distance.begin(),
			sigma_psf_vs_image_sensor_distance.end()
		)
		-
		sigma_psf_vs_image_sensor_distance.begin();

	//sigma_psf_vs_image_sensor_distance
	//image_sensor_distances
	EXPECT_NEAR(0.0, min_sigma_psf, 1e-3);
	EXPECT_NEAR(
		cfg.focal_length, 
		image_sensor_distances.at(min_sigma_psf_pos),
		cfg.focal_length*1e-4
	);
	//std::cout << "smallest psf sigma of "<<min_sigma_psf*1e3<<"mm at d=";
	//std::cout << image_sensor_distances.at(min_sigma_psf_pos)*1e3<<"mm\n";
}