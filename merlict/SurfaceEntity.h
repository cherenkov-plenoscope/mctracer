// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_SURFACEENTITY_H_
#define CORE_SURFACEENTITY_H_

#include <string>
#include "merlict/Frame.h"
#include "Color.h"
#include "merlict/function/function.h"

namespace merlict {

class SurfaceEntity :public Frame {
    const Frame* allowed_frame_to_propagate_to = &VOID_FRAME;
    bool _boundary_layer_is_transparent;
    const Color* outer_color;
    const Color* inner_color;
    const function::Func1* outer_reflection_vs_wavelength;
    const function::Func1* inner_reflection_vs_wavelength;
    const function::Func1* outer_refraction_vs_wavelength;
    const function::Func1* inner_refraction_vs_wavelength;
    const function::Func1* outer_absorption_vs_wavelength;
    const function::Func1* inner_absorption_vs_wavelength;

 public:
    static const SurfaceEntity VOID_SURFACE_ENTITY;
    static const SurfaceEntity PHOTON_SOURCE;

    SurfaceEntity();
    SurfaceEntity(
        const std::string new_name,
        const Vec3 new_pos,
        const Rot3 new_rot);
    void set_outer_color(const Color* color);
    void set_inner_color(const Color* color);
    void set_outer_reflection(const function::Func1* refl);
    void set_inner_reflection(const function::Func1* refl);
    void set_outer_refraction(const function::Func1* refrac);
    void set_inner_refraction(const function::Func1* refrac);
    void set_outer_absorption(const function::Func1* absorp);
    void set_inner_absorption(const function::Func1* absorp);
    void set_allowed_frames_to_propagate_to(const Frame* frame);
    void take_boundary_layer_properties_from(const SurfaceEntity* proto);
    void take_boundary_layer_properties_but_inside_out_from(
        const SurfaceEntity* proto);
    const Color* get_outer_color()const;
    const Color* get_inner_color()const;
    const function::Func1* get_outer_reflection_()const;
    const function::Func1* get_inner_reflection_()const;
    const function::Func1* get_outer_refraction_()const;
    const function::Func1* get_inner_refraction_()const;
    const function::Func1* get_outer_absorption_()const;
    const function::Func1* get_inner_absorption_()const;
    const Frame* get_allowed_frame_to_propagate_to()const;
    bool boundary_layer_is_transparent()const;
    bool has_restrictions_on_frames_to_propagate_to()const;
    std::string str()const;
    static const function::Limits DEFAULT_WAVELENGTH_RANGE;
    static const function::Func1* DEFAULT_REFLECTION;
    static const function::Func1* DEFAULT_REFRACTION;
    static const function::Func1* DEFAULT_ABSORPTION;
    static const Color* DEFAULT_COLOR;

 private:
    void init_surface_defaults();
};

}  // namespace relleums

#endif  // CORE_SURFACEENTITY_H_
