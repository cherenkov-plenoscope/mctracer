//==============================================================================
// include guard
#ifndef __PRNG_Generator_H_INCLUDED__
#define __PRNG_Generator_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <stdint.h>
#include "Core/Vec3.h"
//==============================================================================

// the Pseudo random generators
namespace Random {

	class Generator {
	protected:

		uint32_t seed;
	public:

		uint32_t get_seed()const;
		virtual double uniform() = 0;
		virtual void set_seed(const uint32_t seed);
		void set_seed_now_using_system_clock();
		Vec3 get_point_on_xy_disc_within_radius(const double r);
		Vec3 get_point_on_xy_disc_within_radius_slow(const double r);
		Vec3 get_point_on_unitsphere_within_polar_distance(
			const double max_dist
		);
		double expovariate(const double rate);
		virtual double normal(const double mean, const double std_dev) = 0;
	};
}
#endif // __PRNG_Generator_H_INCLUDED__