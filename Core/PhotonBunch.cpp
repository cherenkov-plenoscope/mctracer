#include "PhotonBunch.h"
#include <omp.h>
#include <sstream>
#include "TrajectoryFactory.h"

namespace PhotonBunch {
	//--------------------------------------------------------------------------
	// propagation
	//--------------------------------------------------------------------------
	void propagate_photons_in_world_with_settings(
		std::vector<Photon*> *photon_bunch,
		const Frame* world, 
		const TracerSettings* settings
	) {
		if(settings->MultiThread())
			propagate_photons_using_multi_thread(photon_bunch, world, settings);
		else
			propagate_photons_using_single_thread(photon_bunch, world, settings);
	}	
	//--------------------------------------------------------------------------
	void propagate_photons_using_single_thread(
		std::vector<Photon*> *photon_bunch,
		const Frame* world, 
		const TracerSettings* settings
	) {
		PseudoRandomNumberGenerator dice(
			settings->get_pseudo_random_number_seed()
		);

		PropagationEnvironment env;
		env.world_geometry = world;
		env.propagation_options = settings;
		env.random_engine = &dice;

		for(uint i = 0; i<photon_bunch->size(); i++ )
			photon_bunch->at(i)->propagate_in(&env);;
	}
	//--------------------------------------------------------------------------
	void propagate_photons_using_multi_thread(
		std::vector<Photon*> *photon_bunch,
		const Frame* world, 
		const TracerSettings* settings
	) {
		uint i;
		uint number_of_threads;
		uint thread_id, ray_counter;
		std::stringstream out;

		#pragma omp parallel shared(settings,world) private(number_of_threads, thread_id, out, ray_counter)
		{	
			PseudoRandomNumberGenerator dice_for_this_thread_only;
			ray_counter = 0;
			thread_id = omp_get_thread_num();
			number_of_threads = omp_get_num_threads();

			PropagationEnvironment env_for_this_thread_only;
			env_for_this_thread_only.world_geometry = world;
			env_for_this_thread_only.propagation_options = settings;
			env_for_this_thread_only.random_engine = &dice_for_this_thread_only;

			#pragma omp for schedule(dynamic) private(i) 
			for(i = 0; i<photon_bunch->size(); i++ )
			{
				ray_counter++;
				photon_bunch->at(i)->propagate_in(&env_for_this_thread_only);
			}

			
			out << "Thread " << thread_id+1 << "/" << number_of_threads;
			out << " is doing " << ray_counter << "/";
			out << photon_bunch->size() << " photons. ";
			out << "Seed: " << dice_for_this_thread_only.seed() << "\n";
			//cout << out.str();
		}
	}
	//--------------------------------------------------------------------------
	// In Out to raw matrix/table -> AsciiIO can read/write this to text files
	//--------------------------------------------------------------------------
	std::vector<Photon*>* raw_matrix2photons(
		std::vector<std::vector<double>> raw_matrix
	) {
		std::vector<Photon*> *photon_bunch = new std::vector<Photon*>;

		for(std::vector<double> raw_row : raw_matrix)
			photon_bunch->push_back(raw_row2photon(raw_row));

		return photon_bunch;
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> photons2raw_matrix(
		std::vector<Photon*> *photon_bunch
	) {
		std::vector<std::vector<double>> raw_matrix;

		for(Photon* ph : *photon_bunch)
			raw_matrix.push_back(photon2raw_row(ph));

		return raw_matrix;		
	}
	//--------------------------------------------------------------------------
	std::vector<double> photon2raw_row(Photon* ph) {
		std::vector<double> raw_row;
		raw_row.reserve(8);

		raw_row.push_back(double(ph->get_id()));

		raw_row.push_back(ph->Support().x());
		raw_row.push_back(ph->Support().y());
		raw_row.push_back(ph->Support().z());

		raw_row.push_back(ph->Direction().x());
		raw_row.push_back(ph->Direction().y());
		raw_row.push_back(ph->Direction().z());

		raw_row.push_back(ph->get_wavelength());

		return raw_row;
	}
	//--------------------------------------------------------------------------
	Photon* raw_row2photon(std::vector<double> &raw_row) {

		assert_raw_row_size_matches_photon(raw_row);
		const double id = raw_row[0];
		const Vector3D support(raw_row[1], raw_row[2], raw_row[3]);
		const Vector3D direction(raw_row[4], raw_row[5], raw_row[6]);
		const double wavelength = raw_row[7];

		Photon* ph = new Photon(support, direction, wavelength);
		ph->set_id(id);
		return ph;
	}
	//--------------------------------------------------------------------------
	void assert_raw_row_size_matches_photon(std::vector<double> &raw_row) {
		if(raw_row.size() != 8) {
			std::stringstream out;
			out << "PhotonBunch, raw row of doubles to photon.\n";
			out << "Expected row to have exactly 8 columns, but actual it has ";
			out << raw_row.size() << " columns.\n";
			throw TracerException(out.str());
		}
	}
	//--------------------------------------------------------------------------
	// Trajectories
	//--------------------------------------------------------------------------
	Trajectories::Trajectories(std::vector<Photon*> *_photon_bunch) {

		photon_bunch = _photon_bunch;
		number_of_trajectories_handed_out_already = 0;
	}
	//--------------------------------------------------------------------------
	bool Trajectories::has_still_trajectories_left()const {

		return number_of_trajectories_handed_out_already<=photon_bunch->size();
	}	
	//--------------------------------------------------------------------------
	Frame* Trajectories::get_next_trajectoy() {

		TrajectoryFactory factory(
			photon_bunch->at(number_of_trajectories_handed_out_already++)
		);
		return factory.get_trajectory();
	}
	//--------------------------------------------------------------------------
	// testing and probing
	//--------------------------------------------------------------------------
	uint get_number_of_photnons_absorbed_in_object(
		const std::vector<Photon*> *photon_bunch, 
		const Frame* frame
	) {
		uint counter=0;

		for(Photon* photon : *photon_bunch)
			if(photon->get_final_intersection()->get_intersecting_object()==frame)
				counter++;

		return counter;
	}
	//--------------------------------------------------------------------------
	// light source
	//--------------------------------------------------------------------------
	namespace Source {

		std::vector<Photon*> *point_like_towards_z_opening_angle_num_photons(
			const double opening_angle,
			const uint number_of_photons
		) {
			std::vector<Photon*>* photon_bunch = new std::vector<Photon*>;

			Vector3D support = Vector3D::null;

			PseudoRandomNumberGenerator prng(0);
			for(uint i=0; i<number_of_photons; i++) {

				double The = opening_angle*sqrt(prng.uniform());
				double Phi = prng.uniform()*2.0*M_PI;

				Vector3D direction(
					sin(The)*cos(Phi),
					sin(The)*sin(Phi),
					cos(The)
				);

				Photon* photon = new Photon(support, direction, 433e-9);
				photon_bunch->push_back(photon);
			}

			return photon_bunch;
		}
	}
	//--------------------------------------------------------------------------
	void transform_all_photons(
		const HomoTrafo3D Trafo, 
		std::vector<Photon*> *photon_bunch
	) {
		for(Photon* photon : *photon_bunch)
			photon->transform(&Trafo);
	}
	//--------------------------------------------------------------------------
	// delete all history
	//--------------------------------------------------------------------------
	void delete_photons_and_history(std::vector<Photon*> *photon_bunch) {
		for(uint i=0; i<photon_bunch->size(); i++) {
			photon_bunch->at(i)->delete_history();
			delete photon_bunch->at(i);
		}		
	}
}