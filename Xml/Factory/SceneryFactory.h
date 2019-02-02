// Copyright 2014 Sebastian A. Mueller
#ifndef XML_FACTORY_SCENERYFACTORY_H_
#define XML_FACTORY_SCENERYFACTORY_H_

#include <vector>
#include <string>
#include "Xml/Xml.h"
#include "Tools/PathTools.h"
#include "Core/Frame.h"
#include "Plenoscope/PlenoscopeInScenery.h"
#include "Scenery/Scenery.h"
#include "Xml/Factory/FrameFab.h"
#include "Scenery/Primitive/Primitive.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Scenery/SegmentedReflector/SegmentedReflector.h"

namespace Xml {

class SceneryFactory {
 public:
    std::string author;
    std::string comment;
    std::vector<Plenoscope::PlenoscopeInScenery> plenoscopes;
    PathTools::Path xml_path;
    Document xml_doc;
    Scenery* scenery;
    explicit SceneryFactory(const std::string path);
    void append_to_frame_in_scenery(Frame* frame, Scenery* scenery);

 private:
    void make_geometry(Frame* mother, const Node node);
    void add_to_sensors_if_sensitive(Frame* frame, const Node node);
    void add_function(const Node node);
    void add_color(const Node node);
};

const Function::Func1D* surface_refl(
    const Node node,
    const Scenery* scenery
) {
    return scenery->functions.get(
        node.child("set_surface").attribute("reflection_vs_wavelength"));
}

const Function::Func1D* surface_refrac(
    const Node node,
    const Scenery* scenery
) {
    return scenery->functions.get(
        node.child("set_surface").attribute("refraction_vs_wavelength"));
}

const Color* surface_color(
    const Node node,
    Scenery* scenery
) {
    return scenery->colors.get(node.child("set_surface").attribute("color"));
}

Frame* add_Frame(Frame* mother, const Node node) {
    FrameFab fab(node);
    Frame* frame = mother->append<Frame>();
    frame->set_name_pos_rot(fab.name, fab.pos, fab.rot);
    return frame;
}

Frame* add_Disc(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    Disc* disc = mother->append<Disc>();
    disc->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);
    disc->set_inner_color(surface_color(node, scenery));
    disc->set_outer_color(surface_color(node, scenery));
    disc->set_outer_reflection(surface_refl(node, scenery));
    disc->set_inner_reflection(surface_refl(node, scenery));
    disc->set_radius(node.child("set_disc").to_double("radius"));
    return disc;
}

Frame* add_Sphere(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    Sphere* sphere = mother->append<Sphere>();
    sphere->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    sphere->set_inner_color(surface_color(node, scenery));
    sphere->set_outer_color(surface_color(node, scenery));
    sphere->set_outer_reflection(surface_refl(node, scenery));
    sphere->set_inner_reflection(surface_refl(node, scenery));

    if (node.child("set_surface").has_attribute("refraction_vs_wavelength"))
        sphere->set_inner_refraction(surface_refrac(node, scenery));

    sphere->set_radius(
        node.child("set_sphere").to_double("radius"));
    return sphere;
}

Frame* add_Plane(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    Plane* plane = mother->append<Plane>();
    plane->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    plane->set_inner_color(surface_color(node, scenery));
    plane->set_outer_color(surface_color(node, scenery));
    plane->set_outer_reflection(surface_refl(node, scenery));
    plane->set_inner_reflection(surface_refl(node, scenery));
    plane->set_x_y_width(
        node.child("set_plane").to_double("x_width"),
        node.child("set_plane").to_double("y_width"));
    return plane;
}

Frame* add_HexPlane(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    HexPlane* plane = mother->append<HexPlane>();
    plane->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    plane->set_inner_color(surface_color(node, scenery));
    plane->set_outer_color(surface_color(node, scenery));
    plane->set_outer_reflection(surface_refl(node, scenery));
    plane->set_inner_reflection(surface_refl(node, scenery));
    plane->set_outer_hex_radius(
        node.child("set_hex_plane").to_double("outer_hex_radius"));
    return plane;
}

Frame* add_Cylinder(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    Cylinder* cyl = mother->append<Cylinder>();
    cyl->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    cyl->set_inner_color(surface_color(node, scenery));
    cyl->set_outer_color(surface_color(node, scenery));
    cyl->set_outer_reflection(surface_refl(node, scenery));
    cyl->set_inner_reflection(surface_refl(node, scenery));
    cyl->set_cylinder(
        node.child("set_cylinder").to_double("radius"),
        node.child("set_cylinder").to_vec3("start_pos"),
        node.child("set_cylinder").to_vec3("end_pos"));
    return cyl;
}

Frame* add_Annulus(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    Annulus* ann = mother->append<Annulus>();
    ann->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    ann->set_inner_color(surface_color(node, scenery));
    ann->set_outer_color(surface_color(node, scenery));
    ann->set_outer_reflection(surface_refl(node, scenery));
    ann->set_inner_reflection(surface_refl(node, scenery));
    ann->set_outer_inner_radius(
        node.child("set_annulus").to_double("outer_radius"),
        node.child("set_annulus").to_double("inner_radius"));
    return ann;
}

Frame* add_BiConvexLensHex(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    BiConvexLensHexBound* lens = mother->append<BiConvexLensHexBound>();
    lens->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    lens->set_inner_color(surface_color(node, scenery));
    lens->set_outer_color(surface_color(node, scenery));
    lens->set_outer_reflection(surface_refl(node, scenery));
    lens->set_inner_refraction(
        scenery->functions.get(
            node.child("set_medium").attribute("refraction_vs_wavelength")));
    lens->set_curvature_radius_and_outer_hex_radius(
        node.child("set_bi_convex_lens_hexagonal").to_double(
            "curvature_radius"),
        node.child("set_bi_convex_lens_hexagonal").to_double(
            "outer_radius"));
    return lens;
}

Frame* add_Triangle(Frame* mother, const Node node, Scenery *scenery) {
    FrameFab framefab(node);
    Triangle* tri = mother->append<Triangle>();
    tri->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    tri->set_inner_color(surface_color(node, scenery));
    tri->set_outer_color(surface_color(node, scenery));
    tri->set_outer_reflection(surface_refl(node, scenery));
    tri->set_inner_reflection(surface_refl(node, scenery));
    tri->set_corners_in_xy_plane(
        node.child("set_triangle").to_double("Ax"),
        node.child("set_triangle").to_double("Ay"),
        node.child("set_triangle").to_double("Bx"),
        node.child("set_triangle").to_double("By"),
        node.child("set_triangle").to_double("Cx"),
        node.child("set_triangle").to_double("Cy"));
    return tri;
}

Frame* add_SphereCapWithHexagonalBound(
    Frame* mother,
    const Node node,
    Scenery* scenery
) {
    FrameFab framefab(node);
    SphereCapWithHexagonalBound* cap =
        mother->append<SphereCapWithHexagonalBound>();
    cap->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);
    cap->set_inner_color(surface_color(node, scenery));
    cap->set_outer_color(surface_color(node, scenery));
    cap->set_outer_reflection(surface_refl(node, scenery));
    cap->set_inner_reflection(surface_refl(node, scenery));
    cap->set_curvature_radius_and_outer_hex_radius(
        node.child("set_sphere_cap_hexagonal").to_double(
            "curvature_radius"),
        node.child("set_sphere_cap_hexagonal").to_double(
            "outer_radius"));
    return cap;
}

Frame* add_SphereCapWithRectangularBound(
    Frame* mother,
    const Node node,
    Scenery* scenery
) {
    FrameFab framefab(node);
    SphereCapWithRectangularBound* cap =
        mother->append<SphereCapWithRectangularBound>();
    cap->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);
    cap->set_inner_color(surface_color(node, scenery));
    cap->set_outer_color(surface_color(node, scenery));
    cap->set_outer_reflection(surface_refl(node, scenery));
    cap->set_inner_reflection(surface_refl(node, scenery));
    cap->set_curvature_radius_and_x_y_width(
        node.child("set_sphere_cap_rectangular").to_double(
            "curvature_radius"),
        node.child("set_sphere_cap_rectangular").to_double("x_width"),
        node.child("set_sphere_cap_rectangular").to_double("y_width"));
    return cap;
}

Frame* add_SegmentedReflector(
    Frame* mother,
    const Node node,
    Scenery* scenery
) {
    const Node refl = node.child("set_segmented_reflector");
    SegmentedReflector::Config cfg;
    cfg.focal_length = refl.to_double("focal_length");
    cfg.DaviesCotton_over_parabolic_mixing_factor =
        refl.to_double("DaviesCotton_over_parabolic_mixing_factor");
    cfg.max_outer_aperture_radius =
        refl.to_double("max_outer_aperture_radius");
    cfg.min_inner_aperture_radius =
        refl.to_double("min_inner_aperture_radius");
    cfg.facet_inner_hex_radius =
        refl.to_double("facet_inner_hex_radius");
    cfg.gap_between_facets = refl.to_double("gap_between_facets");
    cfg.reflectivity = surface_refl(node, scenery);
    SegmentedReflector::Factory refl_fab(cfg);
    FrameFab fab(node);
    Frame* reflector = mother->append<Frame>();
    reflector->set_name_pos_rot(fab.name, fab.pos, fab.rot);
    refl_fab.add_reflector_mirror_facets_to_frame(reflector);
    return reflector;
}

Frame* add_STL(
    Frame* mother,
    const Node node,
    Scenery *scenery,
    const std::string xml_dirname
) {
    const std::string file = PathTools::join(
        xml_dirname,
        node.child("set_stl").attribute("file"));
    const double scale = node.child("set_stl").to_double("scale");

    FrameFab framefab(node);
    SurfaceEntity* object = mother->append<SurfaceEntity>();
    object->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);
    object->set_inner_color(surface_color(node, scenery));
    object->set_outer_color(surface_color(node, scenery));
    object->set_outer_reflection(surface_refl(node, scenery));
    object->set_inner_reflection(surface_refl(node, scenery));
    StereoLitography::add_stl_to_and_inherit_surface_from_surfac_entity(
        file, object, scale);
    return object;
}

Frame* add_light_field_sensor(
    Frame* mother,
    const Node node,
    Scenery* scenery,
    std::vector<Plenoscope::PlenoscopeInScenery>* plenoscopes
) {
    FrameFab ffab(node);
    Frame* light_field_sensor = mother->append<Frame>();
    light_field_sensor->set_name_pos_rot(ffab.name, ffab.pos, ffab.rot);
    const Node lfs = node.child("set_light_field_sensor");
    Plenoscope::LightFieldSensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        lfs.to_double("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        lfs.to_double("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter =
        Deg2Rad(lfs.to_double("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat =
        Deg2Rad(lfs.to_double("hex_pixel_FoV_flat2flat_deg"));
    config.number_of_paxel_on_pixel_diagonal =
        lfs.to_int("number_of_paxel_on_pixel_diagonal");
    config.housing_overhead =
        lfs.to_double("housing_overhead");
    config.lens_refraction =
        scenery->functions.get(lfs.attribute("lens_refraction_vs_wavelength"));
    // config.lens_absorbtion = &perfect_transparency;
    config.bin_reflection =
        scenery->functions.get(lfs.attribute("bin_reflection_vs_wavelength"));
    Plenoscope::PlenoscopeInScenery pis(config);
    Plenoscope::LightFieldSensor::Factory lfs_factory(
        &pis.light_field_sensor_geometry);
    lfs_factory.add_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);
    pis.frame = light_field_sensor;
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    plenoscopes->push_back(pis);
    return light_field_sensor;
}

Frame* add_light_field_sensor_demonstration(
    Frame* mother,
    const Node node,
    Scenery* scenery,
    std::vector<Plenoscope::PlenoscopeInScenery>* plenoscopes
) {
    FrameFab ffab(node);
    Frame* light_field_sensor = mother->append<Frame>();
    light_field_sensor->set_name_pos_rot(ffab.name, ffab.pos, ffab.rot);
    const Node lfs = node.child("set_light_field_sensor");
    Plenoscope::LightFieldSensor::Config config;
    config.sensor_plane2imaging_system =
        *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        lfs.to_double("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        lfs.to_double("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter =
        Deg2Rad(lfs.to_double("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat =
        Deg2Rad(lfs.to_double("hex_pixel_FoV_flat2flat_deg"));
    config.number_of_paxel_on_pixel_diagonal =
        lfs.to_int("number_of_paxel_on_pixel_diagonal");
    config.housing_overhead = lfs.to_double("housing_overhead");
    config.lens_refraction =
        scenery->functions.get(lfs.attribute("lens_refraction_vs_wavelength"));
    // config.lens_absorbtion = &perfect_transparency;
    config.bin_reflection =
        scenery->functions.get(lfs.attribute("bin_reflection_vs_wavelength"));
    Plenoscope::PlenoscopeInScenery pis(config);
    Plenoscope::LightFieldSensor::Factory lfs_factory(
        &pis.light_field_sensor_geometry);
    lfs_factory.add_demonstration_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);
    pis.frame = light_field_sensor;
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    plenoscopes->push_back(pis);
    return light_field_sensor;
}

}  // namespace Xml

#endif  // XML_FACTORY_SCENERYFACTORY_H_
