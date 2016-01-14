#include "Geometry/SegmentedReflector/Factory.h"
#include "Geometry/SphereCapWithHexagonalBound.h"

namespace SegmentedReflector {

	Factory::Factory(const Config ncfg): 
		cfg(ncfg),
		geometry(ncfg)
	{
		init_facets();
		init_reflector();	
	}

	Frame* Factory::get_reflector() {
		return reflector;
	}

	void Factory::init_facets() {

		std::vector<Vector3D> facet_positions = geometry.facet_positions();

		for(uint i=0; i<facet_positions.size(); i++) {

			SphereCapWithHexagonalBound* facet = new SphereCapWithHexagonalBound;
			facets.push_back(facet);
			
			facet->set_name_pos_rot(
				get_name_of_facet(i),
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

	void Factory::init_reflector() {

		reflector = new Frame("Reflector", Vector3D::null, Rotation3D::null);

		for(Frame* facet : facets)
			reflector->set_mother_and_child(facet);

		reflector->cluster_using_helper_frames();
	}

	std::string Factory::get_name_of_facet(const uint i)const {

		std::stringstream facet_name;
		facet_name << "mirrro_ID_" << i;
		return facet_name.str();
	}
} // SegmentedReflector