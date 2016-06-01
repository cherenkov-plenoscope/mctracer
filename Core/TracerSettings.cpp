#include "TracerSettings.h"
#include "Tools/Tools.h"

const TracerSettings TracerSettings::default_settings = TracerSettings();
//------------------------------------------------------------------------------
TracerSettings::TracerSettings()
{
	use_multithread_when_possible = true;
	max_number_of_interactions_per_photon = 5;
	pseudo_random_number_seed = 0;
}