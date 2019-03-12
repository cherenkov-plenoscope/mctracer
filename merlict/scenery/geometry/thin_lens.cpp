// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/geometry/thin_lens.h"

namespace merlict {
namespace thin_lens {

double get_image_dist_given_focal_and_object_dist(double f, double o) {
    //           1
    //  i = -----------
    //        1     1
    //       --- - ---
    //        f     o
    //
    return 1.0/(1.0/f - 1.0/o);
}

double get_object_dist_given_focal_and_image_dist(double f, double i) {
    //           1
    //  o = -----------
    //        1     1
    //       --- - ---
    //        f     i
    //
    return 1.0/(1.0/f - 1.0/i);
}

double get_focal_dist_given_object_and_image_dist(double o, double i) {
    //           1
    //  f = -----------
    //        1     1
    //       --- + ---
    //        i     o
    //
    return 1.0/(1.0/i + 1.0/o);
}

}  // namespace thin_lens
}  // namespace merlict
