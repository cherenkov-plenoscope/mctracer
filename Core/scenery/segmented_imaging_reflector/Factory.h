//=================================
// include guard
#ifndef __SegmentedReflectorFactory_H_INCLUDED__
#define __SegmentedReflectorFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/scenery/segmented_imaging_reflector/Geometry.h"
#include "Core/Frame.h"

namespace relleums {
namespace segmented_imaging_reflector {

class Factory {

	const Config cfg;
	Geometry geometry;
	std::vector<Frame*> facets;
 public:

	Factory(const Config ncfg);
	void add_reflector_mirror_facets_to_frame(Frame* reflector);
};

}  // segmented_imaging_reflector
}  // namespace relleums

#endif // __SegmentedReflectorFactory_H_INCLUDED__ 
