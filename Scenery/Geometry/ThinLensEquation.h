// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_THINLENSEQUATION_H_
#define SCENERY_GEOMETRY_THINLENSEQUATION_H_

// The thin lens equation
//
//    1     1     1
//   --- = --- + ---
//    f     i     o
//
// f: focal length of imaging system
// i: image distance
// o: object distance

namespace ThinLensEquation {
    double get_image_dist_given_focal_and_object_dist(double f, double o);
    double get_object_dist_given_focal_and_image_dist(double f, double i);
    double get_focal_dist_given_object_and_image_dist(double o, double i);
}

#endif  // SCENERY_GEOMETRY_THINLENSEQUATION_H_
