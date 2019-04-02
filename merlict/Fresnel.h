// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_FRESNELREFRACTIONANDREFLECTION_H_
#define MERLICT_FRESNELREFRACTIONANDREFLECTION_H_

#include "merlict/Vec3.h"

namespace merlict {

class Fresnel{
    // Refraction and reflection
 protected:
    const Vec3 incident_obj_sys;
    const Vec3 normal_obj_sys;
    const double n_from;
    const double n_to;

    double cosI;
    double n_from_over_n_to;
    double sinT2;
    double cosT;

 public:
    Fresnel(
        const Vec3 &incident_obj_sys,
        const Vec3 &normal_obj_sys,
        double n_from,
        double n_to);
    Vec3 get_reflec_dir_in_object_system()const;
    Vec3 get_refrac_dir_in_object_system()const;
    double reflection_propability()const;
};

}  // namespace merlict

#endif  // MERLICT_FRESNELREFRACTIONANDREFLECTION_H_
