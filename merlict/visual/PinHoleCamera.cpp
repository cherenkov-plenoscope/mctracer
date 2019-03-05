// Copyright 2014 Sebastian A. Mueller
#include "merlict/visual/PinHoleCamera.h"
#include <exception>
#include "Tracer.h"
#include "merlict/random/random.h"
#include "merlict/vitaliy_vitsentiy_thread_pool.h"

namespace merlict {
namespace visual {

void PinHoleCamera::set_position_and_orientation(
    const Vec3 cam_pos_in_world,
    const Rot3 cam_rot_in_world
) {
    CameraDevice::set_position_and_orientation(
        cam_pos_in_world,
        cam_rot_in_world);
    update_principal_point_for_current_FoV();
    // calculate sensor vectors
    SensorDirectionHori = __camera2root.get_transformed_orientation(VEC3_UNIT_Y);
    SensorDirectionVert = __camera2root.get_transformed_orientation(VEC3_UNIT_X);
}

void PinHoleCamera::set_field_of_view(const double field_of_view) {
    assert_field_of_view_is_valid(field_of_view);
    __field_of_view = field_of_view;
    update_principal_point_for_current_FoV();
}

void PinHoleCamera::update_principal_point_for_current_FoV() {
    /*
     calculate principal point (intersection of optical axis and
     image sensor plane)
     calculate distance of camera base and principal point
     using field_of_view angle

                 camera base
                      /|  \
                     / |  |
                    /  |  |
                   /   |  |
                  /    |  |
                 /fov/2|  |
                /______|  |
               /       |   \
              /        |   /dist_camera_support_to_principal_point
             /         |  |
            /          |  |
           /       ____|  |
          /       | o  |  |
         /________|____|  /
                principal_point
        \______  ______/
               \/
              row/2
    */
    // distance
    dist_camera_support_to_principal_point =
        ((num_cols/2.0)/tan(__field_of_view/2.0));

    // principal point
    principal_point =
        __optical_axis.direction()*
        dist_camera_support_to_principal_point;
}

std::string PinHoleCamera::get_pin_hole_cam_print()const {
    std::stringstream out;
    out << camera_str();
    out << "| distance camera support to principal point: ";
    out << dist_camera_support_to_principal_point << "m\n";
    return out.str();
}

void PinHoleCamera::print()const {
    std::cout << get_pin_hole_cam_print();
}

CameraRay PinHoleCamera::get_ray_for_pixel_in_row_and_col(
    const int row, const int col
)const {
    return CameraRay(
        __position, get_direction_of_ray_for_pixel(row, col));
}

Vec3 PinHoleCamera::get_direction_of_ray_for_pixel(
    const int row, const int col
)const {
    return get_intersection_of_ray_on_image_sensor_for_pixel(row, col);
}

Vec3 PinHoleCamera::get_intersection_of_ray_on_image_sensor_for_pixel(
    const int row, const int col
)const {
    const int vert_position_on_image_sensor = row - num_rows/2;
    const int hori_position_on_image_sensor = col - num_cols/2;

    return principal_point +
        SensorDirectionVert * vert_position_on_image_sensor +
        SensorDirectionHori * hori_position_on_image_sensor;
}

Color do_one_pixel(
    int id,
    const Frame* world,
    const Config* visual_config,
    random::Mt19937 *prng,
    const unsigned int pixel,
    const PinHoleCamera* cam
) {
    (void)id;
    const unsigned int row = pixel / cam->num_cols;
    const unsigned int col = pixel % cam->num_cols;
    CameraRay cam_ray = cam->get_ray_for_pixel_in_row_and_col(row, col);
    Tracer tracer(&cam_ray, world, visual_config, prng);
    return tracer.color;
}

void PinHoleCamera::acquire_image(
    const Frame* world,
    const Config* visual_config,
    Image* image
) {
    assert_resolution(image);
    const unsigned int num_pixel = num_cols*num_rows;

    uint64_t num_threads = std::thread::hardware_concurrency();
    ctpl::thread_pool pool(num_threads);
    random::Mt19937 prng;
    std::vector<std::future<Color>> results(num_pixel);

    for (uint64_t i = 0; i < num_pixel; ++i) {
        results[i] = pool.push(
            do_one_pixel,
            world,
            visual_config,
            &prng,
            i,
            this);
    }

    for (uint64_t i = 0; i < num_pixel; i ++) {
        const unsigned int row = i / num_cols;
        const unsigned int col = i % num_cols;
        image->set_col_row(col, row, results[i].get());
    }
}

}  // namespace visual
}  // namespace merlict
