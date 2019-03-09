// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/Scenery.h"

namespace merlict {

Scenery::Scenery() {
    root.set_name_pos_rot("root", Vec3(0.0, 0.0, 0.0), Rot3(0.0, 0.0, 0.0));
    current_working_directory = ".";
}

}  // namespace merlict
