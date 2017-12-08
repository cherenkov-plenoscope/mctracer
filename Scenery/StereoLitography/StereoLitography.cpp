#include "StereoLitography.h"
#include "Scenery/Primitive/Triangle.h"
using std::string;
using std::vector;
//------------------------------------------------------------------------------
namespace StereoLitography {
//------------------------------------------------------------------------------
void add_stl_to_and_inherit_surface_from_surfac_entity(
    const string path, 
    SurfaceEntity* proto,
    const double scale
) {
    BinaryReader reader(path);
    vector<Facet> facets = reader.get_facets();

    unsigned int facet_count = 0;
    for(Facet facet: facets) {
        Triangle* tri = proto->append<Triangle>();
        tri->set_name_pos_rot(
            "triangle_"+std::to_string(facet_count++),
            Vec3::ORIGIN, 
            Rot3::UNITY
        );
        tri->set_normal_and_3_vertecies(
            facet.n,
            facet.a*scale,
            facet.b*scale,
            facet.c*scale
        );  
        tri->take_boundary_layer_properties_from(proto);        
    }
}
//------------------------------------------------------------------------------
void add_stl_to_frame(const string path, Frame* proto, const double scale) {
    BinaryReader reader(path);
    vector<Facet> facets = reader.get_facets();

    unsigned int facet_count = 0;
    for(Facet facet: facets) {
        Triangle* tri = proto->append<Triangle>();
        tri->set_name_pos_rot(
            "triangle_"+std::to_string(facet_count++),
            Vec3::ORIGIN, 
            Rot3::UNITY
        );
        tri->set_normal_and_3_vertecies(
            facet.n,
            facet.a*scale,
            facet.b*scale,
            facet.c*scale
        );  
        tri->set_outer_color(&Color::GRAY);
        tri->set_inner_color(&Color::DARK_GRAY);       
    }
}
//------------------------------------------------------------------------------
} // StereoLitography