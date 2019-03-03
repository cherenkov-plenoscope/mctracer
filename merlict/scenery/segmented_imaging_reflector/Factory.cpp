// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/segmented_imaging_reflector/Factory.h"
#include "merlict/scenery/primitive/SphereCapWithHexagonalBound.h"


namespace merlict {
namespace segmented_imaging_reflector {

Factory::Factory(const Config ncfg):
    cfg(ncfg),
    geometry(ncfg)
{}

void Factory::add_to_SurfaceEntity(SurfaceEntity* reflector) {
    std::vector<Vec3> facet_positions = geometry.facet_positions();
    for (unsigned int i = 0; i < facet_positions.size(); i++) {
        SphereCapWithHexagonalBound* facet =
            reflector->add<SphereCapWithHexagonalBound>();
        facet->set_name_pos_rot(
            "mirror_facet_" + std::to_string(i),
            facet_positions.at(i),
            geometry.get_rotation_for_facet_position(facet_positions.at(i)));
        facet->set_curvature_radius_and_outer_hex_radius(
            geometry.focal_length()*2.0,
            geometry.facet_outer_hex_radius());
        facet->take_boundary_layer_properties_from(reflector);
    }
}

}  // namespace segmented_imaging_reflector
}  // namespace merlict
