#include "Core/scenery/SegmentedReflector/Factory.h"
#include "Core/scenery/primitive/SphereCapWithHexagonalBound.h"
using std::vector;
using std::string;

namespace relleums {
namespace SegmentedReflector {

	Factory::Factory(const Config ncfg): 
		cfg(ncfg),
		geometry(ncfg)
	{}

	void Factory::add_reflector_mirror_facets_to_frame(Frame* reflector) {
			
		vector<Vec3> facet_positions = geometry.facet_positions();

		for(unsigned int i=0; i<facet_positions.size(); i++) {

			SphereCapWithHexagonalBound* facet = 
				reflector->append<SphereCapWithHexagonalBound>();

			facet->set_name_pos_rot(
				"mirror_facet_" + std::to_string(i),
				facet_positions.at(i),
				geometry.get_rotation_for_facet_position(facet_positions.at(i))
			);

			facet->set_outer_color(cfg.mirror_color);
			facet->set_inner_color(cfg.inner_mirror_color);
			facet->set_outer_reflection(cfg.reflectivity);
			facet->set_curvature_radius_and_outer_hex_radius(
				geometry.focal_length()*2.0,
				geometry.facet_outer_hex_radius()
			);		
		}
	}
}  // SegmentedReflector
}  // namespace relleums
