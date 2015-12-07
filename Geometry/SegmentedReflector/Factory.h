//=================================
// include guard
#ifndef __SegmentedReflectorFactory_H_INCLUDED__
#define __SegmentedReflectorFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Geometry/SegmentedReflector/Geometry.h"
#include "Core/Frame.h"

namespace SegmentedReflector {

	class Factory {

		const Config cfg;
		Geometry geometry;
		std::vector<Frame*> facets;
		Frame* reflector;
	public:

		Factory(const Config ncfg);
		Frame* get_reflector();
	private:

		void init_facets();
		void init_reflector();
		std::string get_name_of_facet(const uint i)const;
	};
} // SegmentedReflector
#endif // __SegmentedReflectorFactory_H_INCLUDED__ 
