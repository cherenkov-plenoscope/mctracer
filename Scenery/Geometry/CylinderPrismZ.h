// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_CYLINDERPRISMZ_H_
#define SCENERY_GEOMETRY_CYLINDERPRISMZ_H_

#include "PrismZ.h"

class CylinderPrismZ :public PrismZ {
 protected:
    double radius_square;

 public:
    void set_radius(const double radius);
    bool is_inside(const Vec3* vec)const;
    double get_radius()const;
    double get_area()const;

 private:
    void assert_radius_is_positiv(const double radius)const;
};

#endif  // SCENERY_GEOMETRY_CYLINDERPRISMZ_H_
