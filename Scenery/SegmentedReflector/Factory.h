//=================================
// include guard
#ifndef __SegmentedReflectorFactory_H_INCLUDED__
#define __SegmentedReflectorFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Scenery/SegmentedReflector/Geometry.h"
#include "Core/Frame.h"

namespace relleums {
namespace SegmentedReflector {

class Factory {

	const Config cfg;
	Geometry geometry;
	std::vector<Frame*> facets;
 public:

	Factory(const Config ncfg);
	void add_reflector_mirror_facets_to_frame(Frame* reflector);
};

}  // SegmentedReflector
}  // namespace relleums

#endif // __SegmentedReflectorFactory_H_INCLUDED__ 
