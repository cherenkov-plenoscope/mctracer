#include "StereoLitographyIo.h"
#include "Geometry/Triangle.h"
//------------------------------------------------------------------------------
namespace StereoLitographyIo {
	Frame* read(const std::string filename, const double scale) {

		BinaryReader reader(filename);

		std::vector<Facet> facets = reader.get_facets();

		return facets_2_mctracer_triangles(facets, scale);
	}
	//--------------------------------------------------------------------------
	Frame* facets_2_mctracer_triangles(
		const std::vector<Facet> facets, 
		const double scale
	) {
		
		Frame* mesh = new Frame("stl_mesh", Vec3::null, Rotation3D::null);

		uint facet_count = 0;

		for(Facet facet : facets) {

			std::stringstream name;
			name << "triangle_" << facet_count++;

			Triangle* tri = new Triangle;

			tri->set_name_pos_rot(
				name.str(),
				Vec3::null, 
				Rotation3D::null
			);

			tri->set_normal_and_3_vertecies(
				facet.n,
				facet.a*scale,
				facet.b*scale,
				facet.c*scale
			);	

			tri->set_outer_color(&Color::gray);
			tri->set_inner_color(&Color::dark_gray);
			
			mesh->set_mother_and_child(tri);
		}

		mesh->init_tree_based_on_mother_child_relations();

		return mesh;
	}
	//--------------------------------------------------------------------------
} // StereoLitographyIo