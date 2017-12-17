// Copyright 2014 Sebastian A. Mueller
#include "Scenery/Scenery.h"

Scenery Scenery::void_scenery = Scenery();

Scenery::Scenery() {
    root.set_name_pos_rot("root", Vec3(0.0, 0.0, 0.0), Rot3(0.0, 0.0, 0.0));
}
