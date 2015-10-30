#include "gtest/gtest.h"
#include "LensMaker.h"
#include "BiConvexLensHexBound.h"
#include "Core/Color.h"
#include <algorithm>

class PlenopticTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(PlenopticTest, check_sebastians_paper_and_pen_calculation) {

	double expected_curvature_radius = 0.125;
	double expected_thickness = 0.0445;
	double refractive_index_pmma = 1.49;
	double focal_length = 0.1335;
	double outer_lens_radius = 0.071;

	EXPECT_NEAR(
		expected_curvature_radius, 
		LensMaker::get_curvature_radius_given_f_r_n(
			focal_length, 
			outer_lens_radius, 
			refractive_index_pmma
		),
		expected_curvature_radius*3e-2
	);

	EXPECT_NEAR(
		expected_thickness, 
		LensMaker::get_lens_thickness_for_R_r(
			expected_curvature_radius, 
			outer_lens_radius
		),
		expected_thickness*3e-2
	);
}
//----------------------------------------------------------------------
TEST_F(PlenopticTest, check_lensmaker_on_optical_table_with_lens) {

	// Hello LensMaker,
	// 
	// we want a lens with
	double focal_length = 1.0;//0.1335;
	// made out of pmma plastic with
	double refractive_index_pmma = 1.49;
	// and an outer aperture radius of 
	double outer_lens_radius = 0.071;
	// please tell us the needed curvature radius.

	double lens_curvature_radius = LensMaker::get_curvature_radius_given_f_r_n(
		focal_length, 
		outer_lens_radius, 
		refractive_index_pmma
	);

	// ok lets test it...
	const Color* lens_col = &Color::gray;
	const Color* sensor_disc_col = &Color::dark_gray;

    const Function::Constant* refraction_vs_wavelength = 
    	new Function::Constant(
    		refractive_index_pmma, 
    		Function::Limits(200e-9, 1200e-9)
    	);

    uint number_of_photons_per_run = 1000;
    std::vector<double> sigma_psf_vs_image_sensor_distance;
    std::vector<double> image_sensor_distances;
	for(
		double offset=-focal_length*0.3;
		offset<focal_length*0.3;
		offset=offset+focal_length*0.01
	) {
		double image_sensor_disc_distance = focal_length + offset;

	    // geometry
		Frame optical_table("table", Vector3D::null, Rotation3D::null);

		BiConvexLensHexBound lens;
		lens.set_name_pos_rot("lens", Vector3D::null, Rotation3D::null);
	    lens.set_outer_color(lens_col);
	    lens.set_inner_color(lens_col);
	    lens.set_inner_refraction(refraction_vs_wavelength);
	    lens.set_curvature_radius_and_outer_hex_radius(
	    	lens_curvature_radius, 
	    	outer_lens_radius
	    );

	    Disc sensor_disc;
	    sensor_disc.set_name_pos_rot(
	    	"sensor_disc", 
	    	Vector3D(0.0, 0.0, -image_sensor_disc_distance), 
	    	Rotation3D::null
	    );
	    sensor_disc.set_outer_color(sensor_disc_col);
	    sensor_disc.set_inner_color(sensor_disc_col);
	    sensor_disc.set_disc_radius(outer_lens_radius*0.85);
	    PhotonSensor sensor(0, &sensor_disc);
	    std::vector<PhotonSensor*> sensor_list = {&sensor};

	    optical_table.set_mother_and_child(&lens);
	    optical_table.set_mother_and_child(&sensor_disc);
	    optical_table.init_tree_based_on_mother_child_relations();

	    // light source
	    std::vector<Photon*>* photons = 
		    PhotonBunch::Source::parallel_towards_z_from_xy_disc(
				outer_lens_radius*0.85, // 0.85 inner hex radius
				number_of_photons_per_run
			);

		HomoTrafo3D Trafo;
		Trafo.set_transformation(
			Rotation3D(0.0,-Deg2Rad(180.0),0.0), 
			Vector3D(0.0, 0.0 ,2.0)
		);

		PhotonBunch::transform_all_photons(Trafo, photons);

		// propagation settings
		TracerSettings settings;	
		settings.SetMultiThread(true);
		settings.SetStoreOnlyLastIntersection(false);

		// photon propagation
		PhotonBunch::propagate_photons_in_world_with_settings(
			photons, &optical_table, &settings
		);

		// detect photons in sensors
		PhotonSensors::reset_all_sesnors(&sensor_list);
		PhotonSensors::assign_photons_to_sensors(photons, &sensor_list);

		std::vector<std::vector<double>> intersects = 
			sensor.get_arrival_table_x_y_t();

		// mean position of photon spread
		double xm=0.0;
		double ym=0.0;
		for(uint i=0; i<intersects.size(); i++){
			xm = xm + intersects[i][0];
			ym = ym + intersects[i][1];
		}
		xm = xm/intersects.size();
		ym = ym/intersects.size();
		// estimate psf sigma in x and y
		double sx=0.0;
		double sy=0.0;
		for(uint i=0; i<intersects.size(); i++){
			sx = sx + (intersects[i][0] - xm)*(intersects[i][0] - xm);
			sy = sy + (intersects[i][1] - ym)*(intersects[i][1] - ym);
		}
		sx = sx/intersects.size();
		sy = sy/intersects.size();
		sx=sqrt(sx);
		sy=sqrt(sy);

		sigma_psf_vs_image_sensor_distance.push_back(sqrt(sx*sx+sy*sy));
		image_sensor_distances.push_back(image_sensor_disc_distance);

		PhotonBunch::delete_photons_and_history(photons);
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
		focal_length, 
		image_sensor_distances.at(min_sigma_psf_pos),
		focal_length*1e-2
	);
	//std::cout << "smallest psf sigma of "<<min_sigma_psf*1e3<<"mm at d=";
	//std::cout << image_sensor_distances.at(min_sigma_psf_pos)*1e3<<"mm\n";
}