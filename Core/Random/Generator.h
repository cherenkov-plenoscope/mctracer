//==============================================================================
// include guard
#ifndef __PRNG_Generator_H_INCLUDED__
#define __PRNG_Generator_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <iostream>
#include <random>
#include "Core/Vec3.h"
//==============================================================================

// the Pseudo random generators
namespace Random {

	class Generator {
	protected:

		unsigned seed;
	public:

		uint get_seed()const;
		virtual double uniform() = 0;
		virtual void set_seed(const uint _seed);
		void set_seed_now_using_system_clock();
		Vec3 get_point_on_xy_disc_within_radius(const double radius);
		Vec3 get_point_on_unitsphere_within_polar_distance(
			const double max_dist
		);
		double expovariate(const double rate);
		virtual double normal(const double mean, const double std_dev) = 0;
	};
}
#endif // __PRNG_Generator_H_INCLUDED__