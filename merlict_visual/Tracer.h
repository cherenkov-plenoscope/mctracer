// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_TRACER_H_
#define VISUAL_TRACER_H_

#include "CameraRay.h"
#include "Config.h"
#include "merlict/random/random.h"
#include "merlict/Photon.h"
#include "merlict/Frame.h"
#include "merlict/Color.h"
#include "merlict/RayAndFrame.h"
#include "merlict/PropagationEnvironment.h"

namespace merlict {
namespace visual {

const double WAVELENGTH = 533e-9;

class Tracer {
 public:
    random::Mt19937* prng;

    const Frame* scenery;
    const Config* config;

    CameraRay* cray;
    Intersection isec;
    Color color;

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
    void trace_back_beyond_boundary_layer(const Fresnel &fresnel);
    void trace_back_to_sky_dome();
    bool surface_iluminated_by_global_light_source()const;
    bool surface_normal_is_facing_observer(
        const Intersection &intersection)const;
    Color shadow_of_sky_light()const;
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_TRACER_H_
