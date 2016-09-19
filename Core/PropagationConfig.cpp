#include "PropagationConfig.h"
#include "Tools/Tools.h"

const PropagationConfig PropagationConfig::default_config = PropagationConfig();
//------------------------------------------------------------------------------
PropagationConfig::PropagationConfig()
{
	use_multithread_when_possible = true;
	max_number_of_interactions_per_photon = 5;
	pseudo_random_number_seed = 0;
}