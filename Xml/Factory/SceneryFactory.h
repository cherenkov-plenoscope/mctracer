// Copyright 2014 Sebastian A. Mueller
#ifndef XML_FACTORY_SCENERYFACTORY_H_
#define XML_FACTORY_SCENERYFACTORY_H_

#include <vector>
#include <string>
#include "Xml/Xml.h"
#include "Tools/PathTools.h"
#include "Core/Frame.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Plenoscope/PlenoscopeInScenery.h"
#include "Scenery/Scenery.h"
#include "Xml/Factory/FrameFab.h"
#include "Scenery/Primitive/Primitive.h"

namespace Xml {

class SceneryFactory {
 public:
    std::vector<PhotonSensor::Sensor*>* raw_sensors;
    std::vector<Plenoscope::PlenoscopeInScenery> plenoscopes;

    std::string author;
    std::string comment;

    PathTools::Path xml_path;
    Document xml_doc;

    Scenery* scenery;

    explicit SceneryFactory(const std::string path);
    PhotonSensor::Sensors sensors()const;
    void append_to_frame_in_scenery(Frame* frame, Scenery* scenery);

 private:
    void make_geometry(Frame* mother, const Node node);
    Frame* add_Cylinder(Frame* mother, const Node node);
    Frame* add_Annulus(Frame* mother, const Node node);
    Frame* add_BiConvexLensHex(Frame* mother, const Node node);
    Frame* add_SphereCapWithHexagonalBound(Frame* mother, const Node node);
    Frame* add_SphereCapWithRectangularBound(Frame* mother, const Node node);
    Frame* add_Triangle(Frame* mother, const Node node);
    Frame* add_SegmentedReflector(Frame* mother, const Node node);
    Frame* add_STL(Frame* mother, const Node node);
    Frame* add_light_field_sensor(Frame* mother, const Node node);
    Frame* add_light_field_sensor_demonstration(Frame* mother, const Node node);
    void add_to_sensors_if_sensitive(Frame* frame, const Node node);
    void add_to_array_if_telescope(Frame* frame, const Node node);

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

}  // namespace Xml

#endif  // XML_FACTORY_SCENERYFACTORY_H_
