// Copyright 2018 Sebastian A. Mueller
#include "json.h"
#include <fstream>
#include <sstream>
#include "nlohmann_json.hpp"
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

void assert_key(const nl::json &j, const string &key) {
    if (!has(j, key)) {
        std::stringstream info;
        info << "Expected key: '" << key << "' in " << j << "\n";
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
    string name = g<string>(j, "name");
    Vec3 pos = as_vec3(j, "pos");
    Rot3 rot;
    if (has(j, "rot")) {
        rot = to_rot3(j["rot"]);
    } else if (has(j, "rot_axis")) {
        const Vec3 rot_axis = as_vec3(j, "rot_axis");
        const double angle = g<double>(j, "rot_angle");
        rot = Rot3(rot_axis, angle);}
    f->set_name_pos_rot(name, pos, rot);
}

void set_surface(SurfaceEntity *s, Scenery *scenery, const nl::json &j) {
    assert_key(j, "surface");
    const nl::json &js = j["surface"];
    if (has(js, "inner_color"))
        s->set_inner_color(
            scenery->colors.get(js["inner_color"]));
    if (has(js, "outer_color"))
        s->set_outer_color(
            scenery->colors.get(js["outer_color"]));
    if (has(js, "inner_reflection"))
        s->set_inner_reflection(
            scenery->functions.get(js["inner_reflection"]));
    if (has(js, "outer_reflection"))
        s->set_outer_reflection(
            scenery->functions.get(js["outer_reflection"]));
    if (has(js, "inner_refraction"))
        s->set_inner_refraction(
            scenery->functions.get(js["inner_refraction"]));
    if (has(js, "outer_refraction"))
        s->set_outer_refraction(
            scenery->functions.get(js["outer_refraction"]));
}

void make_children(Frame* mother, Scenery* scenery, const nl::json &j) {
    for (auto &jchild: j) {
        assert_key(jchild, "type");
        assert_key(jchild, "children");
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
        } else if (is_equal(type, "Disc")) {
            make_children(
                add_Disc(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "Triangle")) {
            make_children(
                add_Triangle(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "StereoLitography")) {
            make_children(
                add_StereoLitography(mother, scenery, jchild),
                scenery,
                jchild["children"]);
        } else if (is_equal(type, "SegmentedReflector")) {
            make_children(
                add_SegmentedReflector(mother, scenery, jchild),
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
    for (auto& jcolor : j) {
        colors->add(
            as_string(jcolor, "name"),
            as_color(jcolor, "rgb"));
    }
}

void add_functions(FunctionMap* functions, const nl::json &j) {
    for (auto &jfun: j) {
        string name = as_string(jfun, "name");
        assert_key(jfun, "argument_versus_value");
        std::vector<std::vector<double>> argument_versus_value;
        for (uint64_t i = 0; i < jfun["argument_versus_value"].size(); i++) {
            if (jfun["argument_versus_value"].at(i).size() != 2) {
                std::stringstream info;
                info << "Expected argument_versus_value in ";
                info << "linear_interpolation to be a list ";
                info << "of length-two-lists.\n";
                info << j << "\n";
                throw std::invalid_argument(info.str());}
            argument_versus_value.push_back({
                jfun["argument_versus_value"].at(i).at(0),
                jfun["argument_versus_value"].at(i).at(1)});}
        Function::LinInterpol* f = functions->add<Function::LinInterpol>(name);
        f->init(argument_versus_value);
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
    assert_key(j, "functions");
    add_functions(&scenery->functions, j["functions"]);

    assert_key(j, "colors");
    add_colors(&scenery->colors, j["colors"]);

    assert_key(j, "children");
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
    sphere->set_radius(g<double>(j, "radius"));
    set_surface(sphere, scenery, j);
    return sphere;
}

Frame* add_StereoLitography(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j)
{
    SurfaceEntity* object = mother->append<SurfaceEntity>();
    set_frame(object, j);
    set_surface(object, scenery, j);
    const double scale = g<double>(j, "scale");
    const string stl_path = PathTools::join(
        scenery->current_working_directory,
        g<string>(j, "path"));
    StereoLitography::add_stl_to_and_inherit_surface_from_surfac_entity(
        stl_path, object, scale);
    return object;
}

Annulus* add_Annulus(Frame* mother, Scenery *scenery, const nl::json &j) {
    Annulus* annulus = mother->append<Annulus>();
    set_frame(annulus, j);
    annulus->set_outer_inner_radius(
        g<double>(j, "outer_radius"),
        g<double>(j, "inner_radius"));
    set_surface(annulus, scenery, j);
    return annulus;
}

Cylinder* add_Cylinder(Frame* mother, Scenery *scenery, const nl::json &j) {
    Cylinder* c = mother->append<Cylinder>();
    if (has(j, "rot")) {
        set_frame(c, j);
        c->set_radius_and_length(
            g<double>(j, "radius"),
            g<double>(j, "length"));
    } else if(has(j, "start_pos")) {
        c->set_name_pos_rot(g<string>(j, "name"), Vec3(0, 0 ,0), Rot3(0, 0, 0));
        c->set_cylinder(
            g<double>(j, "radius"),
            as_vec3(j, "start_pos"),
            as_vec3(j, "end_pos"));
    } else {
        std::stringstream info;
        info << "Expected Cylinder to have either 'pos', 'rot', 'radius', ";
        info << "and 'length', or to have ";
        info << "'start_pos', 'end_pos', and 'radius'. ";
        info << "But actually it has: " << j << "\n";
        throw std::invalid_argument(info.str());
    }
    set_surface(c, scenery, j);
    return c;
}

Triangle* add_Triangle(Frame* mother, Scenery *scenery, const nl::json &j) {
    Triangle* tri = mother->append<Triangle>();
    set_frame(tri, j);
    tri->set_corners_in_xy_plane(
        g<double>(j, "Ax"),
        g<double>(j, "Ay"),
        g<double>(j, "Bx"),
        g<double>(j, "By"),
        g<double>(j, "Cx"),
        g<double>(j, "Cy"));
    set_surface(tri, scenery, j);
    return tri;
}

Disc* add_Disc(Frame* mother, Scenery *scenery, const nl::json &j) {
    Disc* disc = mother->append<Disc>();
    set_frame(disc, j);
    disc->set_radius(g<double>(j, "radius"));
    set_surface(disc, scenery, j);
    return disc;
}

Plane* add_Plane(Frame* mother, Scenery *scenery, const nl::json &j) {
    Plane* plane = mother->append<Plane>();
    set_frame(plane, j);
    plane->set_x_y_width(
        g<double>(j, "x_width"),
        g<double>(j, "y_width"));
    set_surface(plane, scenery, j);
    return plane;
}

HexPlane* add_HexPlane(Frame* mother, Scenery *scenery, const nl::json &j) {
    HexPlane* plane = mother->append<HexPlane>();
    set_frame(plane, j);
    plane->set_outer_hex_radius(g<double>(j, "outer_radius"));
    set_surface(plane, scenery, j);
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
        g<double>(j, "curvature_radius"),
        g<double>(j, "outer_radius"));
    set_surface(lens, scenery, j);
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
        g<double>(j, "curvature_radius"),
        g<double>(j, "outer_radius"));
    set_surface(cap, scenery, j);
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
        g<double>(j, "curvature_radius"),
        g<double>(j, "x_width"),
        g<double>(j, "y_width"));
    set_surface(cap, scenery, j);
    return cap;
}

Frame* add_SegmentedReflector(
    Frame* mother,
    Scenery *scenery,
    const nl::json &j)
{
    SegmentedReflector::Config cfg;
    cfg.focal_length = g<double>(j, "focal_length");
    cfg.DaviesCotton_over_parabolic_mixing_factor = g<double>(
        j, "DaviesCotton_over_parabolic_mixing_factor");
    cfg.max_outer_aperture_radius = g<double>(
        j, "max_outer_aperture_radius");
    cfg.min_inner_aperture_radius = g<double>(
        j, "min_inner_aperture_radius");
    cfg.facet_inner_hex_radius = g<double>(
        j, "facet_inner_hex_radius");
    cfg.gap_between_facets = g<double>(
        j, "gap_between_facets");

    assert_key(j, "surface");
    assert_key(j["surface"], "outer_reflection");
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
    Scenery* scenery,
    const nl::json &j,
    std::vector<Plenoscope::PlenoscopeInScenery>* plenoscopes
) {
    Frame* light_field_sensor = mother->append<Frame>();
    set_frame(light_field_sensor, j);
    Plenoscope::LightFieldSensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        g<double>(j, "expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        g<double>(j, "expected_imaging_system_aperture_radius");
    config.max_FoV_diameter =
        Deg2Rad(g<double>(j, "max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat =
        Deg2Rad(g<double>(j, "hex_pixel_FoV_flat2flat_deg"));
    config.number_of_paxel_on_pixel_diagonal =
        g<uint64_t>(j, "number_of_paxel_on_pixel_diagonal");
    config.housing_overhead =
        g<double>(j, "housing_overhead");
    config.lens_refraction = scenery->functions.get(
            g<string>(j, "lens_refraction_vs_wavelength"));
    config.bin_reflection = scenery->functions.get(
            g<string>(j, "bin_reflection_vs_wavelength"));
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
*/

bool has(const nlohmann::json &j, const std::string &key) {
    return j.find(key) != j.end();
}

Color as_color(const nlohmann::json &j, const std::string key) {
    assert_key(j, key);
    return to_color(j[key]);
}

Vec3 as_vec3(const nlohmann::json &j, const std::string key) {
    assert_key(j, key);
    return to_vec3(j[key]);
}

Rot3 as_rot3(const nlohmann::json &j, const std::string key) {
    assert_key(j, key);
    return to_rot3(j[key]);
}

std::string as_string(const nlohmann::json &j, const std::string key) {
    assert_key(j, key);
    return j[key].get<std::string>();
}

Visual::Config load_visual_config(const std::string &path) {
    const nlohmann::json j = load(path);
    return to_visual_config(j, path);
}

Visual::Config to_visual_config(
    const nlohmann::json &j,
    const std::string &path
) {
    Visual::Config cfg;
    cfg.max_interaction_depth = g<uint64_t>(j, "max_interaction_depth");
    cfg.preview.cols = g<uint64_t>(j["preview"], "cols");
    cfg.preview.rows = g<uint64_t>(j["preview"], "rows");
    cfg.preview.scale = g<uint64_t>(j["preview"], "scale");
    cfg.snapshot.cols = g<uint64_t>(j["snapshot"], "cols");
    cfg.snapshot.rows = g<uint64_t>(j["snapshot"], "rows");
    cfg.snapshot.noise_level = g<uint64_t>(j["snapshot"], "noise_level");
    cfg.snapshot.focal_length_over_aperture_diameter =
        g<double>(j["snapshot"], "focal_length_over_aperture_diameter");
    cfg.snapshot.image_sensor_size_along_a_row =
        g<double>(j["snapshot"], "image_sensor_size_along_a_row");
    cfg.global_illumination.on =
        g<bool>(j["global_illumination"], "on");
    cfg.global_illumination.incoming_direction =
        as_vec3(j["global_illumination"], "incoming_direction");
    cfg.photon_trajectories.radius =
        g<double>(j["photon_trajectories"], "radius");

    assert_key(j, "sky_dome");
    const nlohmann::json &skyj = j["sky_dome"];
    string image_path = g<string>(skyj, "path");

    if (image_path.empty()) {
        cfg.sky_dome = Visual::SkyDome(as_color(skyj, "color"));
    } else {
        PathTools::Path jsonpath = PathTools::Path(path);
        cfg.sky_dome = Visual::SkyDome(
            PathTools::join(jsonpath.dirname, image_path));
        cfg.sky_dome.set_background_color(as_color(skyj, "color"));
    }
    return cfg;
}

PropagationConfig load_PropagationConfig(const std::string &path) {
    const nlohmann::json j = load(path);
    return to_PropagationConfig(j);
}

PropagationConfig to_PropagationConfig(const nlohmann::json &j) {
    PropagationConfig cfg;
    cfg.use_multithread_when_possible =
        g<bool>(j, "use_multithread_when_possible");
    cfg.max_number_of_interactions_per_photon =
        g<uint64_t>(j, "max_number_of_interactions_per_photon");
    return cfg;
}

void transform(const nlohmann::json &j, std::vector<Photon> *photons) {
    Vec3 pos = as_vec3(j, "pos");
    Rot3 rot = as_rot3(j, "rot");
    HomTra3 Trafo;
    Trafo.set_transformation(rot, pos);
    Photons::transform_all_photons_multi_thread(Trafo, photons);
}

std::vector<Photon> to_parallel_disc(const nlohmann::json &j) {
    std::vector<Photon> photons =
        Photons::Source::parallel_towards_z_from_xy_disc(
            g<double>(j, "disc_radius"),
            g<uint64_t>(j, "num_photons"));
    return photons;
}

std::vector<Photon> to_pointsource(const nlohmann::json &j) {
    std::vector<Photon> photons =
        Photons::Source::point_like_towards_z_opening_angle_num_photons(
            g<double>(j, "opening_angle"),
            g<uint64_t>(j, "num_photons"));
    return photons;
}

std::vector<Photon> load_photons(const std::string &path) {
    return to_photons(load(path));
}

std::vector<Photon> to_photons(const nlohmann::json &j) {
    std::vector<Photon> photons;
    if (has(j, "point_source"))  {
        photons = to_pointsource(j["point_source"]);
    } else if (has(j, "parallel_disc")) {
        photons = to_parallel_disc(j["parallel_disc"]);
    } else {
        std::stringstream info;
        info << "Expected InternalPhotonSource from json ";
        info << "to be either 'point_source', or 'parallel_disc'.\n";
        throw UnkownTypeOfLightSource(info.str());
    }
    transform(j, &photons);
    return photons;
}

}  // namespace json
}  // namespace mct
