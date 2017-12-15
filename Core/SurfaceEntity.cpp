// Copyright 2014 Sebastian A. Mueller
#include "Core/SurfaceEntity.h"
#include <limits>
#include <sstream>
#include "Core/Function/Limits.h"
#include "Core/Function/ConstantFunction.h"
using Function::Func1D;
using Function::Constant;
using Function::Limits;

const SurfaceEntity SurfaceEntity::VOID_SURFACE_ENTITY;
const SurfaceEntity SurfaceEntity::PHOTON_SOURCE;

const Color* SurfaceEntity::DEFAULT_COLOR = &Color::DARK_GRAY;

const Limits SurfaceEntity::DEFAULT_WAVELENGTH_RANGE = Limits(200e-9, 1200e-9);

const Func1D* SurfaceEntity::DEFAULT_REFLECTION = new Constant(
    0.0,
    DEFAULT_WAVELENGTH_RANGE
);

const Func1D* SurfaceEntity::DEFAULT_REFRACTION = new Constant(
    1.0,
    DEFAULT_WAVELENGTH_RANGE
);

const Func1D* SurfaceEntity::DEFAULT_ABSORPTION = new Constant(
    std::numeric_limits<double>::infinity(),
    DEFAULT_WAVELENGTH_RANGE
);

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
    _boundary_layer_is_transparent = false;

    outer_color = DEFAULT_COLOR;
    inner_color = DEFAULT_COLOR;

    outer_reflection_vs_wavelength = DEFAULT_REFLECTION;
    inner_reflection_vs_wavelength = DEFAULT_REFLECTION;

    outer_refraction_vs_wavelength = DEFAULT_REFRACTION;
    inner_refraction_vs_wavelength = DEFAULT_REFRACTION;

    outer_absorption_vs_wavelength = DEFAULT_ABSORPTION;
    inner_absorption_vs_wavelength = DEFAULT_ABSORPTION;
}

const Func1D* SurfaceEntity::get_outer_reflection_()const {
    return outer_reflection_vs_wavelength;
}

const Func1D* SurfaceEntity::get_inner_reflection_()const {
    return inner_reflection_vs_wavelength;
}

const Func1D* SurfaceEntity::get_outer_refraction_()const {
    return outer_refraction_vs_wavelength;
}

const Func1D* SurfaceEntity::get_inner_refraction_()const {
    return inner_refraction_vs_wavelength;
}

const Func1D* SurfaceEntity::get_outer_absorption_()const {
    return outer_absorption_vs_wavelength;
}

const Func1D* SurfaceEntity::get_inner_absorption_()const {
    return inner_absorption_vs_wavelength;
}

void SurfaceEntity::set_outer_reflection(const Func1D* reflec) {
    outer_reflection_vs_wavelength = reflec;
}

void SurfaceEntity::set_inner_reflection(const Func1D* reflec) {
    inner_reflection_vs_wavelength = reflec;
}

void SurfaceEntity::set_outer_refraction(const Func1D* refrac) {
    outer_refraction_vs_wavelength = refrac;
    if (outer_refraction_vs_wavelength != DEFAULT_REFRACTION)
        _boundary_layer_is_transparent = true;
}

void SurfaceEntity::set_inner_refraction(const Func1D* refrac) {
    inner_refraction_vs_wavelength = refrac;
    if (inner_refraction_vs_wavelength != DEFAULT_REFRACTION)
        _boundary_layer_is_transparent = true;
}

void SurfaceEntity::set_outer_absorption(const Func1D* absorp) {
    outer_absorption_vs_wavelength = absorp;
}

void SurfaceEntity::set_inner_absorption(const Func1D* absorp) {
    inner_absorption_vs_wavelength = absorp;
}

void SurfaceEntity::set_allowed_frames_to_propagate_to(
    const Frame* allowed_frame_to_propagate_to
) {
    this->allowed_frame_to_propagate_to = allowed_frame_to_propagate_to;
}

bool SurfaceEntity::has_restrictions_on_frames_to_propagate_to()const {
    return allowed_frame_to_propagate_to != &Frame::VOID_FRAME;
}

const Frame* SurfaceEntity::get_allowed_frame_to_propagate_to()const {
    return allowed_frame_to_propagate_to;
}

void SurfaceEntity::set_outer_color(const Color* color) {
    outer_color = color;
}

void SurfaceEntity::set_inner_color(const Color* color) {
    inner_color = color;
}

const Color* SurfaceEntity::get_outer_color()const {
    return outer_color;
}

const Color* SurfaceEntity::get_inner_color()const {
    return inner_color;
}

bool SurfaceEntity::boundary_layer_is_transparent()const {
    return _boundary_layer_is_transparent;
}

void SurfaceEntity::take_boundary_layer_properties_from(
    const SurfaceEntity* proto
) {
    set_outer_color(proto->get_outer_color());
    set_inner_color(proto->get_inner_color());
    set_outer_reflection(proto->get_outer_reflection_());
    set_inner_reflection(proto->get_inner_reflection_());
    set_outer_refraction(proto->get_outer_refraction_());
    set_inner_refraction(proto->get_inner_refraction_());
    set_outer_absorption(proto->get_outer_absorption_());
    set_inner_absorption(proto->get_inner_absorption_());
}

void SurfaceEntity::take_boundary_layer_properties_but_inside_out_from(
    const SurfaceEntity* proto
) {
    set_outer_color(proto->get_inner_color());
    set_inner_color(proto->get_outer_color());
    set_outer_reflection(proto->get_inner_reflection_());
    set_inner_reflection(proto->get_outer_reflection_());
    set_outer_refraction(proto->get_inner_refraction_());
    set_inner_refraction(proto->get_outer_refraction_());
    set_outer_absorption(proto->get_inner_absorption_());
    set_inner_absorption(proto->get_outer_absorption_());
}

std::string SurfaceEntity::str()const {
    std::stringstream out;
    out << Frame::str();

    out << " inner surface:\n";
    out << "| color : " << inner_color->str() << "\n";
    out << "| reflec: " << inner_reflection_vs_wavelength->str() << "\n";
    out << "| refrac: " << inner_refraction_vs_wavelength->str() << "\n";
    out << "| absorp: " << inner_absorption_vs_wavelength->str() << "\n";

    out << " outer surface:\n";
    out << "| color : " << outer_color->str() << "\n";
    out << "| reflec: " << outer_reflection_vs_wavelength->str() << "\n";
    out << "| refrac: " << outer_refraction_vs_wavelength->str() << "\n";
    out << "| absorp: " << inner_absorption_vs_wavelength->str() << "\n";

    out << " boundary layer: ";
    if (_boundary_layer_is_transparent)
        out << "transparent" << "\n";
    else
        out << "opaque" << "\n";

    if (has_restrictions_on_frames_to_propagate_to())
        out << " inner frame: " << allowed_frame_to_propagate_to->
            get_path_in_tree_of_frames() << "\n";

    return out.str();
}
