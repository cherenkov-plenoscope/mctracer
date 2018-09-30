// Copyright 2014 Sebastian A. Mueller
#include "Visual/PinHoleCamera.h"
#include <exception>
#include "Tracer.h"
#include "Core/Random/Random.h"

namespace Visual {

void PinHoleCamera::update_position_and_orientation(
    const Vec3 cam_pos_in_world,
    const Rot3 cam_rot_in_world
) {
    set_position_and_orientation(cam_pos_in_world, cam_rot_in_world);
    update_optical_axis_and_orientation();
    update_principal_point_for_current_FoV();
    // calculate sensor vectors
    SensorDirectionHori = camera2root.get_transformed_orientation(Vec3::UNIT_Y);
    SensorDirectionVert = camera2root.get_transformed_orientation(Vec3::UNIT_X);
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
        ((num_pixel_columns/2.0)/tan(field_of_view/2.0));

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
    const int vert_position_on_image_sensor = row - num_pixel_rows/2;
    const int hori_position_on_image_sensor = col - num_pixel_columns/2;

    return principal_point +
        SensorDirectionVert * vert_position_on_image_sensor +
        SensorDirectionHori * hori_position_on_image_sensor;
}

void PinHoleCamera::acquire_image(
    const Frame* world,
    const Config* visual_config,
    Image* image
) {
    assert_resolution(image);
    unsigned int i, row, col;
    CameraRay cam_ray;
    int HadCatch = 0;
    Random::Mt19937 prng;
    const unsigned int number_pixel = num_pixel_columns*num_pixel_rows;

    #pragma omp parallel shared(visual_config, world, HadCatch) private(i, cam_ray, row, col)
    {
        #pragma omp for schedule(dynamic)
        for (i = 0; i < number_pixel; i++) {
            try {
                row = i / num_pixel_columns;
                col = i % num_pixel_columns;
                cam_ray = get_ray_for_pixel_in_row_and_col(row, col);
                Tracer tracer(&cam_ray, world, visual_config, &prng);
                image->set_col_row(col, row, tracer.color);
            } catch (std::exception &error) {
                HadCatch++;
                std::cerr << error.what();
            } catch (...) {
                HadCatch++;
            }
        }
    }

    if (HadCatch) {
        std::stringstream info;
        info << "PinHoleCamera::" << __func__ << "() in " << __FILE__ << ", ";
        info << __LINE__ << "\n";
        info << "Cought exception during multithread ray tracing.\n";
        throw std::runtime_error(info.str());
    }
}

}  // namespace Visual
