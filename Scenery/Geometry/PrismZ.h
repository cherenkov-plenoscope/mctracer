// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_PRISMZ_H_
#define SCENERY_GEOMETRY_PRISMZ_H_

#include "Core/Vec3.h"

class PrismZ {
 public:
    virtual bool is_inside(const Vec3* vec)const = 0;
    virtual double get_area()const {return 0.0;}
};

#endif  // SCENERY_GEOMETRY_PRISMZ_H_
