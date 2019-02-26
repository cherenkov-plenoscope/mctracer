// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RAY_H_
#define CORE_RAY_H_

#include <string>
#include "merlict/Vec3.h"
#include "merlict/HomTra3.h"

namespace merlict {

class Ray {
 protected:
    Ray();
    Vec3 support_;
    Vec3 direction_;

 public:
    Ray(const Vec3 support, const Vec3 direction);
    void set_support_and_direction(const Vec3 nsup, const Vec3 ndir);
    Vec3 support()const;
    Vec3 direction()const;
    Vec3 position_at(const double scalar)const;
    void transform(const HomTra3 *T);
    void transform_inverse(const HomTra3 *T);
    std::string str()const;
    double parameter_for_closest_distance_to_point(const Vec3 &point)const;
    double closest_distance_to_point(const Vec3 &point)const;
};

}  // namespace relleums

#endif  // CORE_RAY_H_
