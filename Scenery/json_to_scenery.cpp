// Copyright 2018 Sebastian A. Mueller
#include "Scenery/json_to_scenery.h"
#include <fstream>
#include <sstream>
#include "Scenery/json.hpp"
#include "Tools/StringTools.h"
#include "Scenery/Primitive/Primitive.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Tools/PathTools.h"
#include "Scenery/SegmentedReflector/SegmentedReflector.h"
namespace nl = nlohmann;
using std::string;
using StringTools::is_equal;

namespace mct {
namespace json {

nl::json load(const string &path) {
    std::ifstream fin(path.c_str());
    if (fin.is_open()) {
        nl::json content;
        fin >> content;
        fin.close();
        return content;
    } else {
        std::stringstream info;
        info << "json_to_scenery: ";
        info << "Can not open '" << path << "'.";
        throw std::runtime_error(info.str());
    }
}

void assert_key(const nl::json &j, const string &key, const string &obj) {
    if (j.find(key) == j.end()) {
        std::stringstream info;
        info << "Expected key: '" << key << "' in object: '" << obj << "', ";
        info << "but actually it is not.\n";
        info << j << "\n";
        throw MissingKey(info.str());
    }
}

Color to_color(const nl::json &j) {
    try {
        return Color(
            j.at(0).get<float>(),
            j.at(1).get<float>(),
            j.at(2).get<float>());
    } catch (...) {
        std::stringstream info;
        info << "Expected color-rgb to be an array of three floats: ";
        info << "\"rgb\": [r, g, b]', but actually it is" << j << "'.\n";
        throw BadColor(info.str());
    }
}

Function::Limits to_limits(const nl::json &j) {
    try {
        return Function::Limits(
            j.at(0).get<double>(),
            j.at(1).get<double>());
    } catch (...) {
        std::stringstream info;
        info << "Expected limits to be an array of two floats: ";
        info << "\"limits\": [lower, upper]', but actually it is" << j << "'.\n";
        throw BadLimit(info.str());
    }
}

Vec3 to_vec3(const nl::json &j) {
    try {
        return Vec3(
            j.at(0).get<double>(),
            j.at(1).get<double>(),
            j.at(2).get<double>());
    } catch (...) {
        std::stringstream info;
        info << "Expected position to be an array of three floats: ";
        info << "\"pos\": [x, y, z]', but actually it is" << j << "'.\n";
        throw BadVec3(info.str());
    }
}

Rot3 to_rot3(const nl::json &j) {
    try {
        return Rot3(
            j.at(0).get<double>(),
            j.at(1).get<double>(),
            j.at(2).get<double>());
    } catch (...) {
        std::stringstream info;
        info << "Expected rotation to be an array of three floats: ";
        info << "\"rot\": [rx, ry, rz]', but actually it is" << j << "'.\n";
        throw BadRot3(info.str());
    }
}

void set_frame(Frame *f, const nl::json &j) {
    assert_key(j, "name", "frame-child");
    assert_key(j, "pos", "frame-child");
    assert_key(j, "rot", "frame-child");
    f->set_name_pos_rot(j["name"], to_vec3(j["pos"]), to_rot3(j["rot"]));
}

void set_surface(SurfaceEntity *s, Scenery *scenery, const nl::json &j) {
    if (j.find("inner_color") != j.end())
        s->set_inner_color(
            scenery->colors.get(j["inner_color"]));
    if (j.find("outer_color") != j.end())
        s->set_outer_color(
            scenery->colors.get(j["outer_color"]));
    if (j.find("inner_reflection") != j.end())
        s->set_inner_reflection(
            scenery->functions.get(j["inner_reflection"]));
    if (j.find("outer_reflection") != j.end())
        s->set_outer_reflection(
            scenery->functions.get(j["outer_reflection"]));
    if (j.find("inner_refraction") != j.end())
        s->set_inner_refraction(
            scenery->functions.get(j["inner_refraction"]));
    if (j.find("outer_refraction") != j.end())
        s->set_outer_refraction(
            scenery->functions.get(j["outer_refraction"]));
}

void make_children(Frame* mother, Scenery* scenery, const nl::json &j) {
    for (auto &jchild: j) {
        assert_key(jchild, "type", "frame-child");
        assert_key(jchild, "children", "frame-child");
        string type = jchild["type"];

        if (is_equal(type, "Frame")) {
            make_children(
                add_Frame(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "Sphere")) {
            make_children(
                add_Sphere(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "StereoLitography")) {
            make_children(
                add_StereoLitography(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "Annulus")) {
            make_children(
                add_Annulus(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "Cylinder")) {
            make_children(
                add_Cylinder(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "Plane")) {
            make_children(
                add_Plane(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "HexPlane")) {
            make_children(
                add_HexPlane(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "BiConvexLensHex")) {
            make_children(
                add_BiConvexLensHex(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "SphereCapWithHexagonalBound")) {
            make_children(
                add_SphereCapWithHexagonalBound(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "SphereCapWithRectangularBound")) {
            make_children(
                add_SphereCapWithRectangularBound(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        }
    }
}

void add_colors(ColorMap* colors, const nl::json &j) {
    for (auto &col: j.items())
        colors->add(col.key(), to_color(col.value()));
}

void add_functions(FunctionMap* functions, const nl::json &j) {
    for (auto &fun: j.items()) {
        string name = fun.key();
        nl::json v = fun.value();
        assert_key(v, "type", "function");
        if (is_equal(v["type"].get<string>(), "constant")) {
            Function::Constant* f = functions->add<Function::Constant>(name);
            assert_key(v, "value", "constant-function");
            assert_key(v, "limits", "constant-function");
            f->init(v["value"].get<double>(), to_limits(v["limits"]));
        }
    }
}

void append_to_frame_in_scenery(
    Frame* mother,
    Scenery* scenery,
    const string &path
) {
    PathTools::Path json_path(path);
    scenery->current_working_directory = json_path.dirname;
    nl::json j = load(path);
    append_to_frame_in_scenery(mother, scenery, j);
}

void append_to_frame_in_scenery(
    Frame* mother,
    Scenery* scenery,
    const nl::json &j
) {
    // functions
    assert_key(j, "functions", "scenery");
    add_functions(&scenery->functions, j["functions"]);

    // colors
    assert_key(j, "colors", "scenery");
    add_colors(&scenery->colors, j["colors"]);

    // children
    assert_key(j, "children", "scenery");
    make_children(mother, scenery, j["children"]);
}

Frame* add_Frame(Frame* mother, Scenery *scenery, const nl::json &j) {
    (void)scenery;
    Frame* frame = mother->append<Frame>();
    set_frame(frame, j);
    return frame;
}

Sphere* add_Sphere(Frame* mother, Scenery *scenery, const nl::json &j) {
    Sphere* sphere = mother->append<Sphere>();
    set_frame(sphere, j);
    assert_key(j, "radius", "Sphere");
    sphere->set_radius(j["radius"].get<double>());
    assert_key(j, "surface", "Sphere");
    set_surface(sphere, scenery, j["surface"]);
    return sphere;
}

Frame* add_StereoLitography(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j)
{
    SurfaceEntity* object = mother->append<SurfaceEntity>();
    set_frame(object, j);
    assert_key(j, "surface", "StereoLitography");
    set_surface(object, scenery, j["surface"]);
    assert_key(j, "scale", "StereoLitography");
    const double scale = j["scale"].get<double>();
    assert_key(j, "path", "StereoLitography");
    const string stl_path = PathTools::join(
        scenery->current_working_directory, j["path"]);
    StereoLitography::add_stl_to_and_inherit_surface_from_surfac_entity(
        stl_path, object, scale);
    return object;
}

Annulus* add_Annulus(Frame* mother, Scenery *scenery, const nl::json &j) {
    Annulus* annulus = mother->append<Annulus>();
    set_frame(annulus, j);
    assert_key(j, "outer_radius", "Annulus");
    assert_key(j, "inner_radius", "Annulus");
    annulus->set_outer_inner_radius(
        j["outer_radius"].get<double>(),
        j["inner_radius"].get<double>());
    assert_key(j, "surface", "Annulus");
    set_surface(annulus, scenery, j["surface"]);
    return annulus;
}

Cylinder* add_Cylinder(Frame* mother, Scenery *scenery, const nl::json &j) {
    Cylinder* c = mother->append<Cylinder>();
    if (j.find("rot") != j.end()) {
        set_frame(c, j);
        assert_key(j, "radius", "Cylinder-with_explicit-rot");
        assert_key(j, "length", "Cylinder-with_explicit-rot");
        c->set_radius_and_length(
            j["radius"].get<double>(),
            j["length"].get<double>());
    } else if(j.find("start_pos") != j.end()) {
        assert_key(j, "end_pos", "Cylinder-with-start_pos");
        assert_key(j, "name", "Cylinder-with-start_pos");
        assert_key(j, "radius", "Cylinder-with-start_pos");
        c->set_name_pos_rot(j["name"], Vec3(0, 0 ,0), Rot3(0, 0, 0));
        c->set_cylinder(
            j["radius"].get<double>(),
            to_vec3(j["start_pos"]),
            to_vec3(j["end_pos"]));
    } else {
        std::stringstream info;
        info << "Expected Cylinder to have either 'pos', 'rot', 'radius', ";
        info << "and 'length', or to have ";
        info << "'start_pos', 'end_pos', and 'radius'. ";
        info << "But actually it has: " << j << "\n";
        throw std::invalid_argument(info.str());
    }
    assert_key(j, "surface", "Cylinder");
    set_surface(c, scenery, j["surface"]);
    return c;
}

Triangle* add_Triangle(Frame* mother, Scenery *scenery, const nl::json &j) {
    Triangle* tri = mother->append<Triangle>();
    set_frame(tri, j);
    assert_key(j, "Ax", "Triangle"); assert_key(j, "Ay", "Triangle");
    assert_key(j, "Bx", "Triangle"); assert_key(j, "By", "Triangle");
    assert_key(j, "Cx", "Triangle"); assert_key(j, "Cy", "Triangle");
    tri->set_corners_in_xy_plane(
        j["Ax"].get<double>(), j["Ay"].get<double>(),
        j["Bx"].get<double>(), j["By"].get<double>(),
        j["Cx"].get<double>(), j["Cy"].get<double>());
    assert_key(j, "surface", "triangle");
    set_surface(tri, scenery, j["surface"]);
    return tri;
}

Disc* add_Disc(Frame* mother, Scenery *scenery, const nl::json &j) {
    Disc* disc = mother->append<Disc>();
    set_frame(disc, j);
    assert_key(j, "radius", "Disc");
    disc->set_radius(j["radius"].get<double>());
    assert_key(j, "surface", "Disc");
    set_surface(disc, scenery, j["surface"]);
    return disc;
}

Plane* add_Plane(Frame* mother, Scenery *scenery, const nl::json &j) {
    Plane* plane = mother->append<Plane>();
    set_frame(plane, j);
    assert_key(j, "x_width", "Plane");
    assert_key(j, "y_width", "Plane");
    plane->set_x_y_width(
        j["x_width"].get<double>(),
        j["y_width"].get<double>());
    assert_key(j, "surface", "Plane");
    set_surface(plane, scenery, j["surface"]);
    return plane;
}

HexPlane* add_HexPlane(Frame* mother, Scenery *scenery, const nl::json &j) {
    HexPlane* plane = mother->append<HexPlane>();
    set_frame(plane, j);
    assert_key(j, "outer_radius", "HexPlane");
    plane->set_outer_hex_radius(j["outer_radius"].get<double>());
    assert_key(j, "surface", "HexPlane");
    set_surface(plane, scenery, j["surface"]);
    return plane;
}

BiConvexLensHexBound* add_BiConvexLensHex(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j
) {
    BiConvexLensHexBound* lens = mother->append<BiConvexLensHexBound>();
    set_frame(lens, j);
    assert_key(j, "curvature_radius", "BiConvexLensHexBound");
    assert_key(j, "outer_radius", "BiConvexLensHexBound");
    lens->set_curvature_radius_and_outer_hex_radius(
        j["curvature_radius"].get<double>(),
        j["outer_radius"].get<double>());
    assert_key(j, "surface", "lens");
    set_surface(lens, scenery, j["surface"]);
    return lens;
}

SphereCapWithHexagonalBound* add_SphereCapWithHexagonalBound(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j
) {
    SphereCapWithHexagonalBound* cap =
        mother->append<SphereCapWithHexagonalBound>();
    set_frame(cap, j);
    assert_key(j, "curvature_radius", "SphereCapWithHexagonalBound");
    assert_key(j, "outer_radius", "SphereCapWithHexagonalBound");
    cap->set_curvature_radius_and_outer_hex_radius(
        j["curvature_radius"].get<double>(),
        j["outer_radius"].get<double>());
    assert_key(j, "surface", "SphereCapWithHexagonalBound");
    set_surface(cap, scenery, j["surface"]);
    return cap;
}

SphereCapWithRectangularBound* add_SphereCapWithRectangularBound(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j)
{
    SphereCapWithRectangularBound* cap =
        mother->append<SphereCapWithRectangularBound>();
    set_frame(cap, j);
    assert_key(j, "curvature_radius", "SphereCapWithRectangularBound");
    assert_key(j, "x_width", "SphereCapWithRectangularBound");
    assert_key(j, "y_width", "SphereCapWithRectangularBound");
    cap->set_curvature_radius_and_x_y_width(
        j["curvature_radius"].get<double>(),
        j["x_width"].get<double>(),
        j["y_width"].get<double>());
    assert_key(j, "surface", "SphereCapWithRectangularBound");
    set_surface(cap, scenery, j["surface"]);
    return cap;
}

Frame* add_SegmentedReflector(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j)
{
    SegmentedReflector::Config cfg;
    cfg.focal_length = j["focal_length"].get<double>();
    assert_key(
        j, "DaviesCotton_over_parabolic_mixing_factor", "SegmentedReflector");
    cfg.DaviesCotton_over_parabolic_mixing_factor =
        j["DaviesCotton_over_parabolic_mixing_factor"].get<double>();

    assert_key( j, "max_outer_aperture_radius", "SegmentedReflector");
    cfg.max_outer_aperture_radius =
        j["max_outer_aperture_radius"].get<double>();

    assert_key(j, "min_inner_aperture_radius", "SegmentedReflector");
    cfg.min_inner_aperture_radius =
        j["min_inner_aperture_radius"].get<double>();

    assert_key(j, "facet_inner_hex_radius", "SegmentedReflector");
    cfg.facet_inner_hex_radius =
        j["facet_inner_hex_radius"].get<double>();

    assert_key(j, "gap_between_facets", "SegmentedReflector");
    cfg.gap_between_facets =
        j["gap_between_facets"].get<double>();

    assert_key(j, "surface", "SegmentedReflector");
    assert_key(j["surface"], "reflectivity", "surface");
    cfg.reflectivity = scenery->functions.get(
        j["surface"]["reflectivity"]);

    SegmentedReflector::Factory refl_fab(cfg);
    Frame* reflector = mother->append<Frame>();
    set_frame(reflector, j);
    refl_fab.add_reflector_mirror_facets_to_frame(reflector);
    return reflector;
}


/*

Frame* add_light_field_sensor(
    Frame* mother,
    const Node node,
    Scenery* scenery,
    std::vector<Plenoscope::PlenoscopeInScenery>* plenoscopes);
Frame* add_light_field_sensor_demonstration(
*/

}  // namespace json
}  // namespace mct
