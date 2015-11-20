#include "SegmentedReflector.h"

namespace SegmentedReflector {

	InputCard::InputCard(): 
		// Default is HEGRA CT3 aka FACT
		focal_length(4.889),
		DaviesCotton_over_parabolic_mixing_factor(0.5),
		max_outer_aperture_radius(2.1),
		min_inner_aperture_radius(0.2),
		facet_inner_hex_radius(0.30),
		gap_between_facets(0.01)
	{}
} // SegmentedReflector