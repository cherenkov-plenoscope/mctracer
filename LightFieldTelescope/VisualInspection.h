//=================================
// include guard
#ifndef __LightFieldTelescopeVisualInspectiony_H_INCLUDED__
#define __LightFieldTelescopeVisualInspectiony_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/LightFieldTelescope.h"
#include "Geometry/Disc.h"

namespace LightFieldTelescope {

	class VisualInspection {

		Frame world;
		Disc ground;
	public:
		const Factory *fab;
		VisualInspection(const Factory *factory);
	private:

		void set_up_world_basics();
	};
}
#endif // __LightFieldTelescopeVisualInspectiony_H_INCLUDED__ 
