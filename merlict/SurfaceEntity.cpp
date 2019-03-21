// Copyright 2014 Sebastian A. Mueller
#include "merlict/SurfaceEntity.h"
#include <limits>
#include <sstream>

namespace merlict {

const SurfaceEntity VOID_SURFACE_ENTITY = SurfaceEntity();

const function::Func1* SurfaceEntity::DEFAULT_REFLECTION = new function::Func1(
    {
        {200e-9, 0},
        {1200e-9, 0}
    });

const function::Func1* SurfaceEntity::DEFAULT_REFRACTION = new function::Func1(
    {
        {200e-9, 1},
        {1200e-9, 1}
    });

const function::Func1* SurfaceEntity::DEFAULT_ABSORPTION = new function::Func1(
    {
        {200e-9, std::numeric_limits<double>::infinity()},
        {1200e-9, std::numeric_limits<double>::infinity()}
    });

SurfaceEntity::SurfaceEntity() {
    init_surface_defaults();
}

SurfaceEntity::SurfaceEntity(
    const std::string new_name,
    const Vec3 new_pos,
    const Rot3 new_rot
): Frame() {
    set_name_pos_rot(new_name, new_pos, new_rot);
    init_surface_defaults();
}

void SurfaceEntity::init_surface_defaults() {
    outer_color = &COLOR_DARK_GRAY;
    inner_color = &COLOR_DARK_GRAY;

    outer_reflection = DEFAULT_REFLECTION;
    inner_reflection = DEFAULT_REFLECTION;

    outer_refraction = DEFAULT_REFRACTION;
    inner_refraction = DEFAULT_REFRACTION;

    outer_absorption = DEFAULT_ABSORPTION;
    inner_absorption = DEFAULT_ABSORPTION;
}

void SurfaceEntity::set_allowed_frames_to_propagate_to(const Frame* frame) {
    _allowed_frame_to_propagate_to = frame;
}

bool SurfaceEntity::has_restrictions_on_frames_to_propagate_to()const {
    return _allowed_frame_to_propagate_to != &VOID_FRAME;
}

const Frame* SurfaceEntity::allowed_frame_to_propagate_to()const {
    return _allowed_frame_to_propagate_to;
}

bool SurfaceEntity::boundary_layer_is_transparent()const {
    if (
        outer_refraction != DEFAULT_REFRACTION ||
        inner_refraction != DEFAULT_REFRACTION)
        return true;
    else
        return false;
}

void SurfaceEntity::adopt_surface(
    const SurfaceEntity* proto
) {
    outer_color = proto->outer_color;
    inner_color = proto->inner_color;
    outer_reflection = proto->outer_reflection;
    inner_reflection = proto->inner_reflection;
    outer_refraction = proto->outer_refraction;
    inner_refraction = proto->inner_refraction;
    outer_absorption = proto->outer_absorption;
    inner_absorption = proto->inner_absorption;
}

void SurfaceEntity::adopt_surface_inside_out(
    const SurfaceEntity* proto
) {
    outer_color = proto->inner_color;
    inner_color = proto->outer_color;
    outer_reflection = proto->inner_reflection;
    inner_reflection = proto->outer_reflection;
    outer_refraction = proto->inner_refraction;
    inner_refraction = proto->outer_refraction;
    outer_absorption = proto->inner_absorption;
    inner_absorption = proto->outer_absorption;
}

std::string SurfaceEntity::str()const {
    std::stringstream out;
    out << Frame::str();

    out << " inner surface:\n";
    out << "| color : " << inner_color->str() << "\n";
    out << "| reflec: " << inner_reflection->str() << "\n";
    out << "| refrac: " << inner_refraction->str() << "\n";
    out << "| absorp: " << inner_absorption->str() << "\n";

    out << " outer surface:\n";
    out << "| color : " << outer_color->str() << "\n";
    out << "| reflec: " << outer_reflection->str() << "\n";
    out << "| refrac: " << outer_refraction->str() << "\n";
    out << "| absorp: " << inner_absorption->str() << "\n";

    out << " boundary layer: ";
    if (boundary_layer_is_transparent())
        out << "transparent" << "\n";
    else
        out << "opaque" << "\n";

    if (has_restrictions_on_frames_to_propagate_to())
        out << " inner frame: " << _allowed_frame_to_propagate_to->
            path_in_tree() << "\n";

    return out.str();
}

}  // namespace merlict
