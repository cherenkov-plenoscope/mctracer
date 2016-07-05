#include "Photons.h"
#include <omp.h>
#include <sstream>
//#include "Scenery/TrajectoryFactory.h"
#include "Core/PhotonAndFrame.h"
using std::string;
using std::stringstream;
using std::vector;

namespace Photons {
	//--------------------------------------------------------------------------
	string get_print(const vector<Photon*> *photons) {
		stringstream out;
		out << "Photon bunble "<< photons->size() << "\n";
		for(Photon* photon : *photons)
			out << photon->get_print() << "\n";
		return out.str();	
	}	
	//--------------------------------------------------------------------------
	// propagation
	//--------------------------------------------------------------------------
	void propagate_photons_in_scenery_with_settings(
		vector<Photon*> *photons,
		const Frame* world, 
		const TracerSettings* settings,
		Random::Generator* prng
	) {
		if(settings->use_multithread_when_possible)
			propagate_photons_using_multi_thread(photons, world, settings);
		else
			propagate_photons_using_single_thread(photons, world, settings, prng);
	}	
	//--------------------------------------------------------------------------
	void propagate_photons_using_single_thread(
		vector<Photon*> *photons,
		const Frame* world, 
		const TracerSettings* settings,
		Random::Generator* prng
	) {
		PropagationEnvironment env;
		env.world_geometry = world;
		env.propagation_options = settings;
		env.random_engine = prng;

		for(uint i = 0; i<photons->size(); i++ )
			PhotonAndFrame::Propagator(photons->at(i), env);
	}
	//--------------------------------------------------------------------------
	void propagate_photons_using_multi_thread(
		vector<Photon*> *photons,
		const Frame* world, 
		const TracerSettings* settings
	) {
		uint i;
		uint number_of_threads;
		uint thread_id, ray_counter;
		stringstream out;
		int HadCatch = 0;

		#pragma omp parallel shared(settings,world,HadCatch) private(number_of_threads, thread_id, out, ray_counter)
		{	
			Random::Mt19937 dice_for_this_thread_only;
			ray_counter = 0;
			thread_id = omp_get_thread_num();
			number_of_threads = omp_get_num_threads();

			PropagationEnvironment env_for_this_thread_only;
			env_for_this_thread_only.world_geometry = world;
			env_for_this_thread_only.propagation_options = settings;
			env_for_this_thread_only.random_engine = &dice_for_this_thread_only;

			#pragma omp for schedule(dynamic) private(i)
			for(i = 0; i<photons->size(); i++ ) {
				try {
					ray_counter++;
					PhotonAndFrame::Propagator(
						photons->at(i), 
						env_for_this_thread_only
					);
				}catch(std::exception &error) {
					HadCatch++;
					std::cerr << error.what(); 
				}catch(...)	{
					HadCatch++;
				}
			}

			out << "Thread " << thread_id+1 << "/" << number_of_threads;
			out << " is doing " << ray_counter << "/";
			out << photons->size() << " photons. ";
			out << "Seed: " << dice_for_this_thread_only.get_seed() << "\n";
			//cout << out.str();
		}

		if(HadCatch) {
			stringstream info;
			info << "PhotonBunch::"<<__func__<<"() in "<<__FILE__<<", "<<__LINE__<<"\n";
			info << "Cought exception during multithread propagation.\n";
			throw(TracerException(info.str()));
		}
	}
	/*
	A throw executed inside a parallel region must cause execution to resume
	within the same parallel region, and it must be caught by the same thread
	that threw the exception
	*/
	//--------------------------------------------------------------------------
	// In Out to raw matrix/table -> AsciiIO can read/write this to text files
	//--------------------------------------------------------------------------
	vector<Photon*>* raw_matrix2photons(vector<vector<double>> raw_matrix) {
		vector<Photon*> *photons = new vector<Photon*>;

		for(vector<double> raw_row : raw_matrix)
			photons->push_back(raw_row2photon(raw_row));

		return photons;
	}
	//--------------------------------------------------------------------------
	vector<vector<double>> photons2raw_matrix(vector<Photon*> *photons) {
		vector<vector<double>> raw_matrix;

		for(Photon* ph : *photons)
			raw_matrix.push_back(photon2raw_row(ph));

		return raw_matrix;		
	}
	//--------------------------------------------------------------------------
	vector<double> photon2raw_row(Photon* ph) {
		vector<double> raw_row;
		raw_row.reserve(8);

		raw_row.push_back(double(ph->get_id()));

		raw_row.push_back(ph->get_support().x());
		raw_row.push_back(ph->get_support().y());
		raw_row.push_back(ph->get_support().z());

		raw_row.push_back(ph->get_direction().x());
		raw_row.push_back(ph->get_direction().y());
		raw_row.push_back(ph->get_direction().z());

		raw_row.push_back(ph->get_wavelength());

		return raw_row;
	}
	//--------------------------------------------------------------------------
	Photon* raw_row2photon(vector<double> &raw_row) {

		assert_raw_row_size_matches_photon(raw_row);
		const double id = raw_row[0];
		const Vec3 support(raw_row[1], raw_row[2], raw_row[3]);
		const Vec3 direction(raw_row[4], raw_row[5], raw_row[6]);
		const double wavelength = raw_row[7];

		Photon* ph = new Photon(support, direction, wavelength);
		ph->set_id(id);
		return ph;
	}
	//--------------------------------------------------------------------------
	void assert_raw_row_size_matches_photon(vector<double> &raw_row) {
		if(raw_row.size() != 8) {
			stringstream out;
			out << "PhotonBunch, raw row of doubles to photon.\n";
			out << "Expected row to have exactly 8 columns, but actual it has ";
			out << raw_row.size() << " columns.\n";
			throw TracerException(out.str());
		}
	}
	//--------------------------------------------------------------------------
	// Trajectories
	//--------------------------------------------------------------------------
	/*Trajectories::Trajectories(
		vector<Photon*> *_photons,
		const TracerSettings *settings
	) {
		this->settings = settings;
		photons = _photons;
		number_of_trajectories_handed_out_already = 0;
	}
	//--------------------------------------------------------------------------
	bool Trajectories::has_still_trajectories_left()const {

		return number_of_trajectories_handed_out_already<photons->size();
	}	
	//--------------------------------------------------------------------------
	Frame* Trajectories::get_next_trajectoy() {

		TrajectoryFactory factory(
			photons->at(number_of_trajectories_handed_out_already++)
		);
		factory.set_trajectory_radius(settings->visual.photon_trajectories.radius);

		return factory.get_trajectory();
	}*/
	//--------------------------------------------------------------------------
	// light source
	//--------------------------------------------------------------------------
	namespace Source {

		vector<Photon*> *point_like_towards_z_opening_angle_num_photons(
			const double opening_angle,
			const uint number_of_photons
		) {
			vector<Photon*>* photons = new vector<Photon*>;
			photons->reserve(number_of_photons);

			const Vec3 support = Vec3::null;

			Random::Mt19937 prng(0);

			Vec3 direction;
			for(uint i=0; i<number_of_photons; i++) {

				direction = prng.get_point_on_unitsphere_within_polar_distance(
					opening_angle
				);

				Photon* photon = new Photon(support, direction, 433e-9);
				photons->push_back(photon);
			}
			return photons;
		}
		//----------------------------------------------------------------------
		vector<Photon*> *parallel_towards_z_from_xy_disc(
			const double disc_radius,
			const uint number_of_photons
		) {
			vector<Photon*>* photons = new vector<Photon*>;
			photons->reserve(number_of_photons);

			const Vec3 direction = Vec3::unit_z;

			Random::Mt19937 prng(0);

			Vec3 support;
			for(uint i=0; i<number_of_photons; i++) {

				support = prng.get_point_on_xy_disc_within_radius(disc_radius);
				Photon* photon = new Photon(support, direction, 433e-9);
				photons->push_back(photon);
			}

			return photons;
		}
	}
	//--------------------------------------------------------------------------
	// transformations, move and rotate photons
	//--------------------------------------------------------------------------
	void transform_all_photons(
		const HomTra3 Trafo, 
		vector<Photon*> *photons
	) {
		for(Photon* photon : *photons)
			photon->transform(&Trafo);
	}
	//--------------------------------------------------------------------------
	void transform_all_photons_multi_thread(
		const HomTra3 Trafo, 
		vector<Photon*> *photons
	) {
		uint i;
		int HadCatch = 0;
		#pragma omp parallel shared(photons,HadCatch)
		{	
			#pragma omp for schedule(dynamic) private(i) 
			for(i=0; i<photons->size(); i++) {
				try{
					photons->at(i)->transform(&Trafo);
				}catch(std::exception &error) {
					HadCatch++;
					std::cerr << error.what(); 
				}catch(...)	{
					HadCatch++;
				}
			}
		}

		if(HadCatch) {
			stringstream info;
			info << "PhotonBunch::"<<__func__<<"() in "<<__FILE__<<", "<<__LINE__<<"\n";
			info << "Cought exception during multithread transformation.\n";
			throw(TracerException(info.str()));
		}
	}
	//--------------------------------------------------------------------------
	// delete all history
	//--------------------------------------------------------------------------
	void delete_photons(vector<Photon*> *photons) {
		for(uint i=0; i<photons->size(); i++)
			delete photons->at(i);
	}
}