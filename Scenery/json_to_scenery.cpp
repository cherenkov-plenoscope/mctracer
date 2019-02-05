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
    if (!has(j, key)) {
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
    string name = g<string>(j, "name", "frame-child");
    assert_key(j, "pos", "frame-child");
    Vec3 pos = to_vec3(j["pos"]);
    Rot3 rot;
    if (has(j, "rot")) {
        rot = to_rot3(j["rot"]);
    } else if (has(j, "rot_axis")) {
        const Vec3 rot_axis = to_vec3(j["rot_axis"]);
        const double angle = g<double>(j, "rot_angle", "frame-child-rot_axis");
        rot = Rot3(rot_axis, angle);}
    f->set_name_pos_rot(name, pos, rot);
}

void set_surface(SurfaceEntity *s, Scenery *scenery, const nl::json &j) {
    if (has(j, "inner_color"))
        s->set_inner_color(
            scenery->colors.get(j["inner_color"]));
    if (has(j, "outer_color"))
        s->set_outer_color(
            scenery->colors.get(j["outer_color"]));
    if (has(j, "inner_reflection"))
        s->set_inner_reflection(
            scenery->functions.get(j["inner_reflection"]));
    if (has(j, "outer_reflection"))
        s->set_outer_reflection(
            scenery->functions.get(j["outer_reflection"]));
    if (has(j, "inner_refraction"))
        s->set_inner_refraction(
            scenery->functions.get(j["inner_refraction"]));
    if (has(j, "outer_refraction"))
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
        } else if (is_equal(v["type"].get<string>(), "linear_interpolation")) {
            assert_key(v, "argument_versus_value", "linear_interpolation");
            std::vector<std::vector<double>> argument_versus_value;
            for (uint64_t i = 0; i < v["argument_versus_value"].size(); i++) {
                if (v["argument_versus_value"].at(i).size() != 2) {
                    std::stringstream info;
                    info << "Expected argument_versus_value in ";
                    info << "linear_interpolation to be a list ";
                    info << "of length-two-lists.\n";
                    info << j << "\n";
                    throw std::invalid_argument(info.str());}
                argument_versus_value.push_back({
                    v["argument_versus_value"].at(i).at(0),
                    v["argument_versus_value"].at(i).at(1)});}
            Function::LinInterpol* f = functions->add<Function::LinInterpol>(name);
            f->init(argument_versus_value);
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
    assert_key(j, "functions", "scenery");
    add_functions(&scenery->functions, j["functions"]);

    assert_key(j, "colors", "scenery");
    add_colors(&scenery->colors, j["colors"]);

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
    sphere->set_radius(g<double>(j, "radius", "Sphere"));
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
    const double scale = g<double>(j, "scale", "StereoLitography");
    const string stl_path = PathTools::join(
        scenery->current_working_directory,
        g<string>(j, "path", "StereoLitography"));
    StereoLitography::add_stl_to_and_inherit_surface_from_surfac_entity(
        stl_path, object, scale);
    return object;
}

Annulus* add_Annulus(Frame* mother, Scenery *scenery, const nl::json &j) {
    Annulus* annulus = mother->append<Annulus>();
    set_frame(annulus, j);
    annulus->set_outer_inner_radius(
        g<double>(j, "outer_radius", "Annulus"),
        g<double>(j, "inner_radius", "Annulus"));
    assert_key(j, "surface", "Annulus");
    set_surface(annulus, scenery, j["surface"]);
    return annulus;
}

Cylinder* add_Cylinder(Frame* mother, Scenery *scenery, const nl::json &j) {
    Cylinder* c = mother->append<Cylinder>();
    if (has(j, "rot")) {
        set_frame(c, j);
        assert_key(j, "radius", "Cylinder-with_explicit-rot");
        assert_key(j, "length", "Cylinder-with_explicit-rot");
        c->set_radius_and_length(
            j["radius"].get<double>(),
            j["length"].get<double>());
    } else if(has(j, "start_pos")) {
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
    tri->set_corners_in_xy_plane(
        g<double>(j, "Ax", "Triangle"),
        g<double>(j, "Ay", "Triangle"),
        g<double>(j, "Bx", "Triangle"),
        g<double>(j, "By", "Triangle"),
        g<double>(j, "Cx", "Triangle"),
        g<double>(j, "Cy", "Triangle"));
    assert_key(j, "surface", "triangle");
    set_surface(tri, scenery, j["surface"]);
    return tri;
}

Disc* add_Disc(Frame* mother, Scenery *scenery, const nl::json &j) {
    Disc* disc = mother->append<Disc>();
    set_frame(disc, j);
    disc->set_radius(g<double>(j, "radius", "Disc"));
    assert_key(j, "surface", "Disc");
    set_surface(disc, scenery, j["surface"]);
    return disc;
}

Plane* add_Plane(Frame* mother, Scenery *scenery, const nl::json &j) {
    Plane* plane = mother->append<Plane>();
    set_frame(plane, j);
    plane->set_x_y_width(
        g<double>(j, "x_width", "Plane"),
        g<double>(j, "y_width", "Plane"));
    assert_key(j, "surface", "Plane");
    set_surface(plane, scenery, j["surface"]);
    return plane;
}

HexPlane* add_HexPlane(Frame* mother, Scenery *scenery, const nl::json &j) {
    HexPlane* plane = mother->append<HexPlane>();
    set_frame(plane, j);
    plane->set_outer_hex_radius(g<double>(j, "outer_radius", "HexPlane"));
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
    lens->set_curvature_radius_and_outer_hex_radius(
        g<double>(j, "curvature_radius", "BiConvexLensHexBound"),
        g<double>(j, "outer_radius", "BiConvexLensHexBound"));
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
    cap->set_curvature_radius_and_outer_hex_radius(
        g<double>(j, "curvature_radius", "SphereCapWithHexagonalBound"),
        g<double>(j, "outer_radius", "SphereCapWithHexagonalBound"));
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
    cap->set_curvature_radius_and_x_y_width(
        g<double>(j, "curvature_radius", "SphereCapWithRectangularBound"),
        g<double>(j, "x_width", "SphereCapWithRectangularBound"),
        g<double>(j, "y_width", "SphereCapWithRectangularBound"));
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
    cfg.focal_length = g<double>(j, "focal_length", "SegmentedReflector");
    cfg.DaviesCotton_over_parabolic_mixing_factor = g<double>(
        j, "DaviesCotton_over_parabolic_mixing_factor", "SegmentedReflector");
    cfg.max_outer_aperture_radius = g<double>(
        j, "max_outer_aperture_radius","SegmentedReflector");
    cfg.min_inner_aperture_radius = g<double>(
        j, "min_inner_aperture_radius", "SegmentedReflector");
    cfg.facet_inner_hex_radius = g<double>(
        j, "facet_inner_hex_radius", "SegmentedReflector");
    cfg.gap_between_facets = g<double>(
        j, "gap_between_facets", "SegmentedReflector");

    assert_key(j, "surface", "SegmentedReflector");
    assert_key(j["surface"], "outer_reflection", "SegmentedReflector-surface");
    cfg.reflectivity = scenery->functions.get(
        j["surface"]["outer_reflection"]);

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
