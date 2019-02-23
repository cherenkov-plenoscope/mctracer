// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_TRACER_H_
#define VISUAL_TRACER_H_

#include "CameraRay.h"
#include "Config.h"
#include "Core/Random/Random.h"
#include "Core/Photon.h"
#include "Core/Frame.h"
#include "Core/Color.h"
#include "Core/RayAndFrame.h"
#include "Core/PropagationEnvironment.h"

namespace relleums {
namespace visual {

class Tracer {
 public:
    random::Mt19937* prng;

    const Frame* scenery;
    const Config* config;

    CameraRay* cray;
    Intersection isec;
    Color color;

    static const double wavelength;

    Tracer(
        CameraRay* cray,
        const Frame* scenery,
        const Config* config,
        random::Mt19937* prng);
    void trace_back();
    void trace_back_to_object_interaction();
    void trace_back_after_reflection();
    void trace_back_to_boundary_layer();
    void trace_back_to_fresnel_interaction();
    void trace_back_to_surface();
    void trace_back_beyond_boundary_layer(
        const FresnelRefractionAndReflection &fresnel);
    void trace_back_to_sky_dome();
    bool surface_iluminated_by_global_light_source()const;
    bool surface_normal_is_facing_observer(
        const Intersection &intersection)const;
    Color shadow_of_sky_light()const;
};

}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_TRACER_H_
