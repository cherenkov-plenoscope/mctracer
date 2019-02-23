// Copyright 2014 Sebastian A. Mueller
#include "Visual/PinHoleCamera.h"
#include <exception>
#include "Tracer.h"
#include "Core/random/random.h"
#include "vit_vit_cptl_stl.h"

namespace relleums {
namespace visual {

void PinHoleCamera::update_position_and_orientation(
    const Vec3 cam_pos_in_world,
    const Rot3 cam_rot_in_world
) {
    set_position_and_orientation(cam_pos_in_world, cam_rot_in_world);
    update_optical_axis_and_orientation();
    update_principal_point_for_current_FoV();
    // calculate sensor vectors
    SensorDirectionHori = camera2root.get_transformed_orientation(VEC3_UNIT_Y);
    SensorDirectionVert = camera2root.get_transformed_orientation(VEC3_UNIT_X);
}

void PinHoleCamera::set_FoV_in_rad(const double field_of_view) {
    assert_field_of_view_is_valid(field_of_view);
    this->field_of_view = field_of_view;
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
        ((number_cols/2.0)/tan(field_of_view/2.0));

    // principal point
    principal_point =
        pointing*dist_camera_support_to_principal_point;
}

std::string PinHoleCamera::get_pin_hole_cam_print()const {
    std::stringstream out;
    out << get_camera_print();
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
        position, get_direction_of_ray_for_pixel(row, col));
}

Vec3 PinHoleCamera::get_direction_of_ray_for_pixel(
    const int row, const int col
)const {
    return get_intersection_of_ray_on_image_sensor_for_pixel(row, col);
}

Vec3 PinHoleCamera::get_intersection_of_ray_on_image_sensor_for_pixel(
    const int row, const int col
)const {
    const int vert_position_on_image_sensor = row - number_rows/2;
    const int hori_position_on_image_sensor = col - number_cols/2;

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
    const unsigned int row = pixel / cam->number_cols;
    const unsigned int col = pixel % cam->number_cols;
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
    const unsigned int number_pixel = number_cols*number_rows;

    uint64_t num_threads = std::thread::hardware_concurrency();
    ctpl::thread_pool pool(num_threads);
    random::Mt19937 prng;
    std::vector<std::future<Color>> results(number_pixel);

    for (uint64_t i = 0; i < number_pixel; ++i) {
        results[i] = pool.push(
            do_one_pixel,
            world,
            visual_config,
            &prng,
            i,
            this);
    }

    for (uint64_t i = 0; i < number_pixel; i ++) {
        const unsigned int row = i / number_cols;
        const unsigned int col = i % number_cols;
        image->set_col_row(col, row, results[i].get());
    }
}

}  // namespace visual
}  // namespace relleums
