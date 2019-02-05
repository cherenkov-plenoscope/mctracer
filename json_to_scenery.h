// Copyright 2018 Sebastian A. Mueller
#ifndef SCENERY_JSON_TO_SCENERY_H_
#define SCENERY_JSON_TO_SCENERY_H_

#include <string>
#include "Scenery.h"
#include "Core/Frame.h"
#include "Scenery/Primitive/Primitive.h"
#include "nlohmann_json.hpp"

namespace mct {
namespace json {

void append_to_frame_in_scenery(
    Frame* frame,
    Scenery* scenery,
    const std::string &path);

void append_to_frame_in_scenery(
    Frame* mother,
    Scenery* scenery,
    const nlohmann::json &j);

class MissingKey : public std::invalid_argument {
    using invalid_argument::invalid_argument;};
class BadColor : public std::invalid_argument {
    using invalid_argument::invalid_argument;};
class BadVec3 : public std::invalid_argument {
    using invalid_argument::invalid_argument;};
class BadRot3 : public std::invalid_argument {
    using invalid_argument::invalid_argument;};

nlohmann::json load(const std::string &path);
void assert_key(const nlohmann::json &j, const std::string &key);
Color to_color(const nlohmann::json &j);
Vec3 to_vec3(const nlohmann::json &j);
Rot3 to_rot3(const nlohmann::json &j);
void set_frame(Frame *f, const nlohmann::json &j);
void set_surface(SurfaceEntity *s, Scenery *scenery, const nlohmann::json &j);
void make_children(Frame* mother, Scenery* scenery, const nlohmann::json &j);
void add_colors(ColorMap* colors, const nlohmann::json &j);
void add_functions(FunctionMap* functions, const nlohmann::json &j);

Frame* add_Frame(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Sphere* add_Sphere(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Annulus* add_Annulus(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Cylinder* add_Cylinder(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Triangle* add_Triangle(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Disc* add_Disc(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Plane* add_Plane(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

HexPlane* add_HexPlane(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

Frame* add_StereoLitography(
	Frame* mother,
	Scenery *scenery,
	const nlohmann::json &j);

BiConvexLensHexBound* add_BiConvexLensHex(
    Frame* mother,
    Scenery *scenery,
    const nlohmann::json &j);

SphereCapWithHexagonalBound* add_SphereCapWithHexagonalBound(
    Frame* mother,
    Scenery *scenery,
    const nlohmann::json &j);

SphereCapWithRectangularBound* add_SphereCapWithRectangularBound(
    Frame* mother,
    Scenery *scenery,
    const nlohmann::json &j);

template<class Ret>
Ret g(const nlohmann::json &j, const std::string &key) {
	assert_key(j, key);
	return j[key].get<Ret>();}

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

std::string as_string(const nlohmann::json &j, const std::string key) {
    assert_key(j, key);
    return j[key].get<std::string>();
}

}  // namespace json
}  // namespace mct

#endif  // SCENERY_JSON_TO_SCENERY_H_
