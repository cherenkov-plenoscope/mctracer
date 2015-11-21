#include "LensMaker.h"
#include <sstream>
#include <math.h>
#include "BiConvexLens.h"
#include "BiConvexLensHexBound.h"
#include "Disc.h"
#include "Core/Function/ConstantFunction.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Core/Photons.h"
#include "Cameras/FreeOrbitCamera.h"
//------------------------------------------------------------------------------
//  f: focal length
//  R: curvature radius(absolute the same for both caps of the lens)
//  n: refractive index
//  r: outer aperture radius of the lens
//
//  The lens maker equation
//  1/f = (n-1) [2/R - ((n-1)t)/(nR^2)]
//  
//  thickness of lens
//  t = 2R - 2sqrt(R^2 - r^2)
//------------------------------------------------------------------------------
namespace LensMaker {

	Config::Config() {
		focal_length = 1.0;
		aperture_radius = 0.1;
		refractive_index = 1.33;
	}

	Itterative::Itterative(const Config wishlist):order(wishlist) {

		last_point_spread_std_dev = order.aperture_radius*2.0;

		max_number_of_iterations = 100;
		number_of_photons_per_iteration = 5e3;
	
		settings.SetMultiThread(true);
		settings.store_only_last_intersection(false);

		curvature_radius_1st_guess = Approximation::get_curvature_radius(order);

		// a little bigger to make it shrink in the iteration process
		curvature_radius = 1.05*curvature_radius_1st_guess;

		Function::Limits limits(200e-9, 1200e-9);
		Function::Constant refraction_vs_wavelength(order.refractive_index, limits);

		BiConvexLens lens;
		lens.set_name_pos_rot("prototype", Vector3D::null, Rotation3D::null);
		lens.set_inner_refraction(&refraction_vs_wavelength);
		lens.set_curvature_radius_and_aperture_radius(
			curvature_radius_1st_guess, 
			order.aperture_radius
		);

		Disc image_sensor;
		image_sensor.set_name_pos_rot(
			"sensor_disc", 
			Vector3D(0.0, 0.0, -order.focal_length), 
			Rotation3D::null
		);
		image_sensor.set_disc_radius(order.aperture_radius*2.0); 
	    PhotonSensor::Xy sensor(0, &image_sensor);
	    std::vector<PhotonSensor::Sensor*> sensor_list = {&sensor};

		Frame optical_table("table", Vector3D::null, Rotation3D::null);
		optical_table.set_mother_and_child(&image_sensor);
		optical_table.set_mother_and_child(&lens);
		optical_table.init_tree_based_on_mother_child_relations();

		uint iteration_counter = 0;

		
		double point_spread_std_dev;

		while(true) {
			abort_if_too_many_iterations(iteration_counter);
		
			// modify the lens
			lens.set_curvature_radius_and_aperture_radius(
				curvature_radius, 
				order.aperture_radius
			);
			optical_table.init_tree_based_on_mother_child_relations();

			// light source
		    std::vector<Photon*>* photons = 
			    Photons::Source::parallel_towards_z_from_xy_disc(
					order.aperture_radius,
					number_of_photons_per_iteration
				);

			HomoTrafo3D Trafo;
			Trafo.set_transformation(
				Rotation3D(0.0,-Deg2Rad(180.0),0.0), 
				Vector3D(0.0, 0.0 ,order.focal_length)
			);

			Photons::transform_all_photons(Trafo, photons);

			// photon propagation
			Photons::propagate_photons_in_world_with_settings(
				photons, &optical_table, &settings
			);

			// detect photons in sensors
			PhotonSensors::reset_all_sesnors(&sensor_list);
			PhotonSensors::assign_photons_to_sensors(photons, &sensor_list);

			point_spread_std_dev = sensor.point_spread_std_dev();

			//std::cout << "curvature_radius: " << curvature_radius << "m, psf std dev: "<< point_spread_std_dev <<"m, e: "<< photons->size()<< ", n: "<< sensor.get_arrival_table().size()<<"\n";

			//FreeOrbitCamera free(&optical_table, &settings);

			Photons::delete_photons_and_history(photons);

			if(is_bigger_then_last_psf(point_spread_std_dev))
				break;

			last_curvature_radius = curvature_radius;
			curvature_radius = curvature_radius*0.995;
		}
	}
	//--------------------------------------------------------------------------
	bool Itterative::is_bigger_then_last_psf(const double psf_std_dev) {
		bool is_better = last_point_spread_std_dev < psf_std_dev;
		last_point_spread_std_dev = psf_std_dev;
		return is_better;
	}
	//--------------------------------------------------------------------------
	double Itterative::get_curvature_radius_for_bi_konvex_lens()const {
		return last_curvature_radius;
	}
	//--------------------------------------------------------------------------
	void Itterative::abort_if_too_many_iterations(const uint i)const {

		if(i > max_number_of_iterations) {
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "LensMaker exceeded max number of " << max_number_of_iterations;
			info << " iterations. ";
			info << "Can not reach best curvature radius.\n";
			info << "Input:\n";
			info << "  focal length..........." << order.focal_length <<"m\n";
			info << "  aperture radius........" << order.aperture_radius <<"m\n";
			info << "  refractive index......." << order.refractive_index <<"\n";
			info << "Analytic start of iteration:\n";
			info << "  curvature_radius......." << curvature_radius_1st_guess << "m\n"; 
			throw NoConvergence(info.str());
		}
	}

	namespace Approximation {
	//--------------------------------------------------------------------------
	double get_curvature_radius(const Config &cfg) {

		double R = cfg.focal_length; // iteration start point
		double deviation = 0.0;
		uint iteration_conter = 0;

		do {
			abort_if_too_many_iterations(cfg, ++iteration_conter, R);
			
			// adjustment on curvature radius 
			R = R - deviation*0.1;
			
			// expected focal length for adjusted curvature radius
			double f_expected = get_expected_focal_length_for_R_t_n(
				R, 
				get_lens_thickness_for_R_r(R, cfg.aperture_radius),  
				cfg.refractive_index
			);

			deviation = f_expected - cfg.focal_length;
		}while(fabs(deviation) > 1e-6);

		return R;
	}
	//--------------------------------------------------------------------------
	double get_expected_focal_length_for_R_t_n(
		const double R, const double t, const double n
	) {
		double f_inv = (n-1.0)*( 2.0/R - ((n-1.0)*t)/(n*R*R));
		return 1.0/f_inv;
	}
	//--------------------------------------------------------------------------
	double get_lens_thickness_for_R_r(const double R, const double r) {
		return 2.0*R - 2.0*sqrt(R*R - r*r);
	}
	//--------------------------------------------------------------------------
	void abort_if_too_many_iterations(
		const Config &cfg, 
		const uint iteration_conter,
		const double R
	) {
		const uint max_iterations = 1000;
		if(iteration_conter > max_iterations) {
			std::stringstream info;
			info << "LensMaker::" << __func__ << "()\n";
			info << "Exceeded max number of " << max_iterations;
			info << " iterations. ";
			info << "Can not reach best curvature radius.\n";
			info << "Input:\n";
			info << "  focal length..........." << cfg.focal_length <<"m\n";
			info << "  aperture radius........" << cfg.aperture_radius <<"m\n";
			info << "  refractive index......." << cfg.refractive_index <<"\n";
			info << "Result:\n";
			info << "  lens curvature radius.." << R <<"m\n";
			info << "  lens thickness........." << get_lens_thickness_for_R_r(R,cfg.aperture_radius) <<"m\n";
			throw NoConvergence(info.str());
		}
	}
	//--------------------------------------------------------------------------
	} // Analytic
} // LensMaker