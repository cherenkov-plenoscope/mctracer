// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_RECTANGULARBOX_H_
#define SCENERY_PRIMITIVE_RECTANGULARBOX_H_

#include "Plane.h"

namespace relleums {

class RectangularBox :public SurfaceEntity {
 public:
    void set_xyz_width(
        const double x_width,
        const double y_width,
        const double z_width);
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_RECTANGULARBOX_H_
