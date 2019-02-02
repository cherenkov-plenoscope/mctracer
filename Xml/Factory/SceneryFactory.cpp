// Copyright 2014 Sebastian A. Mueller
#include "Xml/Factory/SceneryFactory.h"
#include <sstream>
#include "FrameFab.h"
#include "Scenery/Primitive/Primitive.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Scenery/SegmentedReflector/SegmentedReflector.h"
#include "Xml/Factory/Function.h"

using StringTools::is_equal;
using std::stringstream;
using std::string;
using std::vector;


namespace Xml {

SceneryFactory::SceneryFactory(const string path):
    xml_path(path),
    xml_doc(path) {
    const Node root_node = xml_doc.node().first_child();
    if (root_node.has_attribute("author"))
        author = root_node.attribute("author");
    if (root_node.has_attribute("comment"))
        comment = root_node.attribute("comment");
    raw_sensors = new vector<PhotonSensor::Sensor*>;
    scenery = &Scenery::void_scenery;
}

void SceneryFactory::append_to_frame_in_scenery(
    Frame* frame,
    Scenery* scenery
) {
    this->scenery = scenery;
    const Node root_node = xml_doc.node().first_child();
    make_geometry(frame, root_node);
}

void SceneryFactory::make_geometry(Frame* mother, const Node node) {
    add_to_sensors_if_sensitive(mother, node);

    for (Node child = node.first_child(); child; child = child.next_child()) {
        if (is_equal(child.name(), "function"))
            add_function(child);
        else if (is_equal(child.name(), "color"))
            add_color(child);
        else if (is_equal(child.name(), "frame"))
            make_geometry(add_Frame(mother, child), child);
        else if (is_equal(child.name(), "disc"))
            make_geometry(add_Disc(mother, child, scenery), child);
        else if (is_equal(child.name(), "sphere"))
            make_geometry(add_Sphere(mother, child, scenery), child);
        else if (is_equal(child.name(), "plane"))
            make_geometry(add_Plane(mother, child, scenery), child);
        else if (is_equal(child.name(), "hex_plane"))
            make_geometry(add_HexPlane(mother, child, scenery), child);
        else if (is_equal(child.name(), "cylinder"))
            make_geometry(add_Cylinder(mother, child, scenery), child);
        else if (is_equal(child.name(), "annulus"))
            make_geometry(add_Annulus(mother, child, scenery), child);
        else if (is_equal(child.name(), "segmented_reflector"))
            make_geometry(add_SegmentedReflector(mother, child), child);
        else if (is_equal(child.name(), "sphere_cap_hexagonal"))
            make_geometry(
                add_SphereCapWithHexagonalBound(mother, child), child);
        else if (is_equal(child.name(), "sphere_cap_rectangular"))
            make_geometry(
                add_SphereCapWithRectangularBound(mother, child), child);
        else if (is_equal(child.name(), "triangle"))
            make_geometry(add_Triangle(mother, child, scenery), child);
        else if (is_equal(child.name(), "light_field_sensor"))
            make_geometry(add_light_field_sensor(mother, child), child);
        else if (is_equal(child.name(), "light_field_sensor_demonstration"))
            make_geometry(
                add_light_field_sensor_demonstration(mother, child), child);
        else if (is_equal(child.name(), "stl"))
            make_geometry(add_STL(mother, child), child);
        else if (is_equal(child.name(), "bi_convex_lens_hexagonal"))
            make_geometry(add_BiConvexLensHex(mother, child, scenery), child);
    }
}

void SceneryFactory::add_to_sensors_if_sensitive(
    Frame* frame,
    const Node node
) {
    if (node.has_child("set_sensitive")) {
        const unsigned int id = node.child("set_sensitive").to_int("id");
        PhotonSensor::Sensor* sens = new PhotonSensor::Sensor(id, frame);
        raw_sensors->push_back(sens);
    }
}

Frame* SceneryFactory::add_SphereCapWithHexagonalBound(
    Frame* mother,
    const Node node
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

Frame* SceneryFactory::add_SphereCapWithRectangularBound(
    Frame* mother,
    const Node node
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

Frame* SceneryFactory::add_STL(Frame* mother, const Node node) {
    const string file = PathTools::join(
        xml_path.dirname,
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

Frame* SceneryFactory::add_SegmentedReflector(Frame* mother, const Node node) {
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

Frame* SceneryFactory::add_light_field_sensor(Frame* mother, const Node node) {
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
    plenoscopes.push_back(pis);

    return light_field_sensor;
}

Frame* SceneryFactory::add_light_field_sensor_demonstration(
    Frame* mother,
    const Node node
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
    plenoscopes.push_back(pis);

    return light_field_sensor;
}

void SceneryFactory::add_color(const Node node) {
    scenery->colors.add(
        node.attribute("name"),
        node.to_color("rgb"));
}

void SceneryFactory::add_function(const Node node) {
    for (
        Xml::Node child = node.first_child();
        child;
        child = child.next_child()
    ) {
        if (is_equal(child.name(), "linear_interpolation")) {
            Function::LinInterpol* f =
                scenery->functions.add<Function::LinInterpol>(
                    node.attribute("name"));
            init_function_with_node(f, child);
        } else if (is_equal(child.name(), "constant")) {
            Function::Constant* f = scenery->functions.add<Function::Constant>(
                node.attribute("name"));
            init_function_with_node(f, child);
        } else if (is_equal(child.name(), "polynom3")) {
            Function::Polynom3* f = scenery->functions.add<Function::Polynom3>(
                node.attribute("name"));
            init_function_with_node(f, child);
        } else if (is_equal(child.name(), "concatenation")) {
            Function::Concat* f = scenery->functions.add<Function::Concat>(
                node.attribute("name"));
            init_function_with_node(f, child, &scenery->functions);
        } else {
            stringstream info;
            info << "SceneryFactory " << __FILE__ << ", " << __LINE__;
            info << "Unknown xml child in '" << child.name() << "' in node ";
            info << "'" << node.name() << "'";
            throw std::invalid_argument(info.str());
        }
    }
}

PhotonSensor::Sensors SceneryFactory::sensors()const {
    return PhotonSensor::Sensors(*raw_sensors);
}

}  // namespace Xml
