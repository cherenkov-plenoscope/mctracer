// Copyright 2018 Sebastian A. Mueller
#ifndef JSON_H_
#define JSON_H_
#include <string>
#include <sstream>
#include <vector>
#include "Scenery/Scenery.h"
#include "Core/Frame.h"
#include "Core/Photons.h"
#include "Core/PropagationConfig.h"
#include "Visual/Config.h"
#include "Scenery/primitive/Primitive.h"
#include "niels_lohmann_json.hpp"

namespace relleums {
namespace json {

void assert_key(const nlohmann::json &j, const std::string &key);
void assert_idx(const nlohmann::json &j, const uint64_t idx);
bool has(const nlohmann::json &j, const std::string &key);

class BadTriple : public std::invalid_argument {
    using invalid_argument::invalid_argument;};

std::array<double, 3> to_triple(const nlohmann::json &j);

class MissingKey : public std::invalid_argument {
    using invalid_argument::invalid_argument;};
class ListTooShort : public std::invalid_argument {
    using invalid_argument::invalid_argument;};

struct Object {
    const nlohmann::json j;
    Object(const nlohmann::json _j);
    bool key(const std::string &key)const;
    double f8(const std::string &key)const;
    double f8(const uint64_t idx)const;
    int64_t i8(const std::string &key)const;
    int64_t i8(const uint64_t idx)const;
    uint64_t u8(const std::string &key)const;
    uint64_t u8(const uint64_t idx)const;
    std::string st(const std::string &key)const;
    std::string st(const uint64_t idx)const;
    Object obj(const std::string &key)const;
    Object obj(const uint64_t idx)const;
    Vec3 vec3(const std::string &key)const;
    Vec3 vec3(const uint64_t idx)const;
    Rot3 rot3(const std::string &key)const;
    Rot3 rot3(const uint64_t idx)const;
    Color color(const std::string &key)const;
    Color color(const uint64_t idx)const;
    bool b1(const std::string &key)const;
    bool b1(const uint64_t idx)const;

    uint64_t size()const;
    std::string str()const;
};

Object load(const std::string &path);

void set_frame(Frame *f, const Object &o);
void set_surface(SurfaceEntity *s, Scenery *scenery, const Object &o);
void make_children(Frame* mother, Scenery* scenery, const Object &o);
void add_colors(ColorMap* colors, const Object &o);
std::vector<std::vector<double>> json_to_vec_of_vecs(const Object &avsv);
function::Func1 json_to_linear_interpol_function(const Object &avsv);
void add_functions(FunctionMap* functions, const Object &o);

void append_to_frame_in_scenery(
    Frame* frame,
    Scenery* scenery,
    const std::string &path);

void append_to_frame_in_scenery(
    Frame* mother,
    Scenery* scenery,
    const Object &o);

Frame* add_Frame(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_Sphere(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_Annulus(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_Cylinder(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_Triangle(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_Disc(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_Plane(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_HexPlane(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_StereoLitography(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_BiConvexLensHex(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_SphereCapWithHexagonalBound(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_SphereCapWithRectangularBound(
    Frame* mother,
    Scenery *scenery,
    const Object &o);

Frame* add_SegmentedReflector(
    Frame* mother,
    Scenery *scenery,
    const Object &o);


typedef Frame* (*json_to_frame)(Frame*, Scenery*, const Object &);

std::map<std::string, json_to_frame> basic_scenery();

bool has_key(std::map<std::string, json_to_frame> m, const std::string &key);

void assert_has_key(std::map<std::string, json_to_frame> m, const std::string &key);

json_to_frame get(std::map<std::string, json_to_frame> m, const std::string &key);

visual::Config to_visual_config(const Object &o, const std::string &path);

visual::Config load_visual_config(const std::string &path);

PropagationConfig to_PropagationConfig(const Object &o);

PropagationConfig load_PropagationConfig(const std::string &path);

void transform_photons(const Object &o, std::vector<Photon> *photons);

std::vector<Photon> to_parallel_disc(const Object &o);

std::vector<Photon> to_pointsource(const Object &o);

class UnkownTypeOfLightSource : public std::invalid_argument {
    using invalid_argument::invalid_argument;};

std::vector<Photon> load_photons(const std::string &path);

std::vector<Photon> to_photons(const Object &o);

}  // namespace json
}  // namespace relleums

#endif  // JSON_H_
