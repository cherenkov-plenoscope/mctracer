// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_PRISMZ_H_
#define SCENERY_GEOMETRY_PRISMZ_H_

#include "Core/Vec3.h"

namespace relleums {

class PrismZ {
 public:
    virtual bool is_inside(const Vec3* vec)const = 0;
    virtual double get_area()const;
};

}  // namespace relleums

#endif  // SCENERY_GEOMETRY_PRISMZ_H_
