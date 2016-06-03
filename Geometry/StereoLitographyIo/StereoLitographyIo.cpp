#include "StereoLitographyIo.h"
#include "Geometry/Triangle.h"
//------------------------------------------------------------------------------
namespace StereoLitographyIo {
//------------------------------------------------------------------------------
void add_stl_to_and_inherit_surface_from_surfac_entity(
    const string path, 
    SurfaceEntity* proto,
    const double scale
) {
    BinaryReader reader(path);
    vector<Facet> facets = reader.get_facets();

    uint facet_count = 0;
    for(Facet facet: facets) {
        Triangle* tri = new Triangle;
        tri->set_name_pos_rot(
            "triangle_"+std::to_string(facet_count++),
            Vec3::null, 
            Rot3::null
        );
        tri->set_normal_and_3_vertecies(
            facet.n,
            facet.a*scale,
            facet.b*scale,
            facet.c*scale
        );  
        tri->take_boundary_layer_properties_from(proto);        
        proto->set_mother_and_child(tri);
    }
    proto->cluster_using_helper_frames();
}
//------------------------------------------------------------------------------
void add_stl_to_frame(const string path, Frame* proto, const double scale) {
    BinaryReader reader(path);
    vector<Facet> facets = reader.get_facets();

    uint facet_count = 0;
    for(Facet facet: facets) {
        Triangle* tri = new Triangle;
        tri->set_name_pos_rot(
            "triangle_"+std::to_string(facet_count++),
            Vec3::null, 
            Rot3::null
        );
        tri->set_normal_and_3_vertecies(
            facet.n,
            facet.a*scale,
            facet.b*scale,
            facet.c*scale
        );  
        tri->set_outer_color(&Color::gray);
        tri->set_inner_color(&Color::dark_gray);       
        proto->set_mother_and_child(tri);
    }
    proto->cluster_using_helper_frames();
}
//------------------------------------------------------------------------------
} // StereoLitographyIo