// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_SURFACEENTITY_H_
#define MERLICT_SURFACEENTITY_H_

#include <string>
#include "merlict/Frame.h"
#include "Color.h"
#include "merlict/function/function.h"

namespace merlict {

class SurfaceEntity :public Frame {
    const Frame* _allowed_frame_to_propagate_to = &VOID_FRAME;
 public:
    static const SurfaceEntity VOID_SURFACE_ENTITY;

    SurfaceEntity();
    SurfaceEntity(
        const std::string new_name,
        const Vec3 new_pos,
        const Rot3 new_rot);

    const Color* outer_color;
    const Color* inner_color;
    const function::Func1* outer_reflection;
    const function::Func1* inner_reflection;
    const function::Func1* outer_refraction;
    const function::Func1* inner_refraction;
    const function::Func1* outer_absorption;
    const function::Func1* inner_absorption;

    void set_allowed_frames_to_propagate_to(const Frame* frame);
    void adopt_surface(const SurfaceEntity* proto);
    void adopt_surface_inside_out(const SurfaceEntity* proto);
    const Frame* allowed_frame_to_propagate_to()const;
    bool boundary_layer_is_transparent()const;
    bool has_restrictions_on_frames_to_propagate_to()const;
    std::string str()const;
    static const function::Func1* DEFAULT_REFLECTION;
    static const function::Func1* DEFAULT_REFRACTION;
    static const function::Func1* DEFAULT_ABSORPTION;

 private:
    void init_surface_defaults();
};

const SurfaceEntity SURFACE_PHOTON_SOURCE;

}  // namespace merlict

#endif  // MERLICT_SURFACEENTITY_H_
