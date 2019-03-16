// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERACARD_H_
#define VISUAL_CAMERACARD_H_

#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include "merlict/Rot3.h"
#include "merlict/Vec3.h"
#include "merlict/HomTra3.h"
#include "CameraRay.h"
#include "Image.h"

namespace merlict {
namespace visual {

struct CameraCard {
    Vec3 position;
    Rot3 orientation;
    double field_of_view;
    uint64_t num_cols;
    uint64_t num_rows;
};

struct CameraPinHole {
    Vec3 position;
    Rot3 orientation;
    double field_of_view;
    uint64_t num_cols;
    uint64_t num_rows;
    uint64_t num_pixel;
    HomTra3 camera2root;
    Ray optical_axis;
    Vec3 principal_point;
    Vec3 sensor_row;
    Vec3 sensor_col;
};

CameraPinHole make_CameraPinHole(const CameraCard &card) {
    CameraPinHole r;
    r.position = card.position;
    r.orientation = card.orientation;
    r.field_of_view = card.field_of_view;
    r.num_cols = card.num_cols;
    r.num_rows = card.num_rows;
    r.num_pixel = r.num_rows*r.num_cols;

    r.camera2root.set_transformation(
        r.rotation,
        r.position);
    pointing = r.camera2root.get_transformed_orientation(
        VEC3_UNIT_Z);
    r.optical_axis.set_support_and_direction(
        r.position,
        pointing);
    r.sensor_row = r.camera2root.get_transformed_orientation(
        VEC3_UNIT_Y);
    r.sensor_col = r.camera2root.get_transformed_orientation(
        VEC3_UNIT_X);

    // calculate principal_point (intersection of optical axis and
    // image sensor plane)
    // calculate distance of camera base and principal point
    // using field_of_view angle
    //
    //             camera base
    //                  /|  \
    //                 / |  |
    //                /  |  |
    //               /   |  |
    //              /    |  |
    //             /fov/2|  |
    //            /______|  |
    //           /       |   \
    //          /        |   /cam_support_to_principal_point
    //         /         |  |
    //        /          |  |
    //       /       ____|  |
    //      /       | o  |  |
    //     /________|____|  /
    //            principal_point
    //    \______  ______/
    //           \/
    //          row/2

    const double cam_support_to_principal_point =
        ((r.num_cols/2.0)/tan(r.field_of_view/2.0));
    r.principal_point = r.optical_axis.direction()*
        cam_support_to_principal_point;

    return r;
}


Vec3 intersection_of_ray_and_sensor_for_row_col(
    const CameraPinHole &cam,
    const int64_t row,
    const int64_t col
) {
    const int64_t row_idx_on_sensor = row - cam.num_rows/2;
    const int64_t col_idx_on_sensor = col - cam.num_cols/2;
    return cam.principal_point +
        cam.sensor_row * row_idx_on_sensor +
        cam.sensor_col * col_idx_on_sensor;
}


CameraRay camera_ray_row_col(
    const CameraPinHole &cam,
    const int64_t row,
    const int64_t col
)const {
    return CameraRay(
        r.position,
        intersection_of_ray_and_sensor_for_row_col(cam, row, col));
}


void aquire_image(
    const CameraPinHole &cam,
    const Frame* world,
    const Config* config,
    random::Generator *prng,
    Image* image
) {
    for (uint64_t i = 0; i < cam.num_pixel; i ++) {
        const uint64_t row = i / cam.num_cols;
        const uint64_t col = i % cam.num_cols;
        CameraRay cam_ray = camera_ray_row_col(cam, row, col);
        Tracer tracer(&cam_ray, world, config, prng);
        image->set_col_row(col, row, tracer.color);
    }
}

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_CAMERACARD_H_
