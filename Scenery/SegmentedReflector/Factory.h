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

namespace SegmentedReflector {

	class Factory {

		const Config cfg;
		Geometry geometry;
		std::vector<Frame*> facets;
	public:

		Factory(const Config ncfg);
		void add_reflector_mirror_facets_to_frame(Frame* reflector);
	private:

		void init_facets();
		std::string get_name_of_facet(const uint i)const;
	};
} // SegmentedReflector
#endif // __SegmentedReflectorFactory_H_INCLUDED__ 
