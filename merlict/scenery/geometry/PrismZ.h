// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_PRISMZ_H_
#define SCENERY_GEOMETRY_PRISMZ_H_

#include "merlict/Vec3.h"

namespace merlict {

class PrismZ {
 public:
    virtual bool is_inside(const Vec3* vec)const = 0;
    virtual double get_area()const;
};

}  // namespace merlict

#endif  // SCENERY_GEOMETRY_PRISMZ_H_
