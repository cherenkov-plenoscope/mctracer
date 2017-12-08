// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_FRESNELREFRACTIONANDREFLECTION_H_
#define MCTRACER_CORE_FRESNELREFRACTIONANDREFLECTION_H_

#include "Core/Vec3.h"

class FresnelRefractionAndReflection{
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
    FresnelRefractionAndReflection(
        const Vec3 &incident_obj_sys,
        const Vec3 &normal_obj_sys,
        double n_from,
        double n_to);
    Vec3 get_reflec_dir_in_object_system()const;
    Vec3 get_refrac_dir_in_object_system()const;
    double reflection_propability()const;
};
#endif  // MCTRACER_CORE_FRESNELREFRACTIONANDREFLECTION_H_
