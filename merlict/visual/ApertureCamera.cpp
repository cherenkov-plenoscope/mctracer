// Copyright 2014 Sebastian A. Mueller
#include "merlict/visual/ApertureCamera.h"
#include <vector>
#include <exception>
#include <future>
#include <thread>
#include "merlict/visual/Tracer.h"
#include "merlict/tools.h"
#include "merlict/random/random.h"
#include "merlict/vitaliy_vitsentiy_thread_pool.h"


namespace merlict {
namespace visual {

double ApertureCamera::f_stop_number()const {return _f_stop_number;}

double ApertureCamera::sensor_width()const {return _sensor_width;}

double ApertureCamera::object_distance()const {return _object_distance;}

void ApertureCamera::set_fstop_sensorwidth_focus(
    const double f_stop_number,
    const double sensor_width,
    const double object_distance
) {
    assert_f_stop_number(f_stop_number);
    _f_stop_number = f_stop_number;

    assert_sensor_width(sensor_width);
    const double width_to_height_ratio =
        static_cast<double>(num_cols)/
        static_cast<double>(num_rows);
    _sensor_width = sensor_width;
    _sensor_height = _sensor_width / width_to_height_ratio;

    _pixel_pitch = _sensor_width/static_cast<double>(num_cols);

    assert_object_distance(object_distance);
    _object_distance = object_distance;

    init_sensor_distance();
}

void ApertureCamera::assert_f_stop_number(const double new_f_stop_number) {
    if (new_f_stop_number <= 0.0) {
        std::stringstream info;
        info << "Expected F-Stop number to be greater zero, but actual it is ";
        info << new_f_stop_number << ".";
        throw std::invalid_argument(info.str());
    }
}

void ApertureCamera::assert_sensor_width(const double width) {
    if (width <= 0.0) {
        std::stringstream info;
        info << "Expected image sensor width to be greater zero, ";
        info << "but actual it is ";
        info << width << ".";
        throw std::invalid_argument(info.str());
    }
}

void ApertureCamera::assert_object_distance(const double new_object_distance) {
    if (new_object_distance <= 0.0) {
        std::stringstream info;
        info << "Expected object distance to focus on to be ";
        info << "greater zero, but actual it is ";
        info << new_object_distance << ".";
        throw std::invalid_argument(info.str());
    }
}

void ApertureCamera::set_pos_rot_fov(
    const Vec3 position,
    const Rot3 rotation,
    const double field_of_view
) {
    CameraDevice::set_pos_rot_fov(position, rotation, field_of_view);

    _focal_length = (_sensor_width/2.0)/tan(__field_of_view/2.0);
    _aperture_radius = _focal_length/(2.0*_f_stop_number);

    init_sensor_distance();
}

void ApertureCamera::init_sensor_distance() {
    // 1/f = 1/b + 1/g
    // f = focal_length
    // b = sensor_distance
    // g = object_distance
    _sensor_distance = 1.0/(1.0/_focal_length - 1.0/_object_distance);
}

std::string ApertureCamera::get_aperture_camera_print()const {
    std::stringstream out;
    out << "| Focal ratio      : " << _f_stop_number << "\n";
    out << "| Focal length     : " << _focal_length*1e3 << " mm\n";
    out << "| Aperture diameter: " << _aperture_radius*2e3 << " mm\n";
    out << "| Sensor size      : ";
    out << 1e3*_sensor_width << " x ";
    out << 1e3*_sensor_height << " mm^2\n";
    out << "| Pixel pitch      : " << _pixel_pitch*1e6 << " um\n";
    out << "| Object distance  : " << _object_distance << " m\n";
    out << "| Sensor distance  : " << _sensor_distance*1e3 << " mm\n";
    return out.str();
}

std::string ApertureCamera::str()const {
    std::stringstream out;
    out << camera_str() << get_aperture_camera_print();
    return out.str();
}

Vec3 ApertureCamera::random_position_on_aperture_disc(
    random::Mt19937 *prng
)const {
    Vec3 ap = prng->get_point_on_xy_disc_within_radius(_aperture_radius);
    return Vec3(ap.x, ap.y, _sensor_distance);
}

Vec3 ApertureCamera::intersection_position_on_object_plane_for_row_col(
    const unsigned int row,
    const unsigned int col,
    random::Mt19937 *prng
)const {
    const int x_pos_on_sensor_in_pixel =  row - num_rows/2;
    const int y_pos_on_sensor_in_pixel =  col - num_cols/2;

    const double image_size_x = x_pos_on_sensor_in_pixel * _pixel_pitch;
    const double image_size_y = y_pos_on_sensor_in_pixel * _pixel_pitch;

    const double sensor_extension_x = (prng->uniform() - .5) * _pixel_pitch;
    const double sensor_extension_y = (prng->uniform() - .5) * _pixel_pitch;

    return Vec3(
        object_size_for_image_size(image_size_x + sensor_extension_x),
        object_size_for_image_size(image_size_y + sensor_extension_y),
        _object_distance);
}

double ApertureCamera::object_size_for_image_size(
    const double image_size_in_m
)const {
    return image_size_in_m * _object_distance / _sensor_distance;
}

CameraRay ApertureCamera::get_ray_for_pixel_in_row_and_col(
    const unsigned int row,
    const unsigned int col,
    random::Mt19937 *prng
)const {
    const Vec3 support_in_cam_frame =
        random_position_on_aperture_disc(prng);
    const Vec3 obj_plane_intersec_in_cam_frame =
        intersection_position_on_object_plane_for_row_col(row, col, prng);
    const Vec3 direction_in_cam_frame =
        obj_plane_intersec_in_cam_frame - support_in_cam_frame;
    return CameraRay(
        ray_support_in_root(support_in_cam_frame),
        ray_direction_in_root(direction_in_cam_frame));
}

Vec3 ApertureCamera::ray_support_in_root(
    const Vec3 &cam_ray_support_in_cam_frame
)const {
    return __position + __camera2root.get_transformed_orientation(
        cam_ray_support_in_cam_frame);
}

Vec3 ApertureCamera::ray_direction_in_root(
    const Vec3 &cam_ray_direction
)const {
    return __camera2root.get_transformed_orientation(cam_ray_direction);
}

void ApertureCamera::acquire_image(
    const Frame* world,
    const Config* visual_config,
    Image* image
) {
    assert_resolution(image);

    const unsigned int rows = num_rows;
    const unsigned int cols = num_cols;
    const unsigned int num_pixel = rows*cols;

    Image sum_image(cols, rows);
    Image exposure_image(cols, rows);

    // initial image
    std::vector<PixelCoordinate> all_pixels = pixel_coordinates(cols, rows);
    std::vector<Color> all_colors = acquire_pixels(
        world,
        visual_config,
        all_pixels);

    assign_pixel_colors_to_sum_and_exposure_image(
        all_pixels,
        all_colors,
        &sum_image,
        &exposure_image);

    image_from_sum_and_exposure(
        sum_image,
        exposure_image,
        image);

    Image sobel_image(cols, rows);
    sobel_operator(*image, &sobel_image);
    truncate_to_255(&sobel_image);

    Image to_do_image(cols, rows);
    luminance_threshold_dilatation(sobel_image, 128., &to_do_image);

    const unsigned int MAX_ITERATIONS = 100;
    unsigned int iteration = 0;
    Image diff_image(cols, rows);
    Image previous_sobel_image(cols, rows);

    while (true) {
        if (iteration >= MAX_ITERATIONS)
            break;

        std::vector<PixelCoordinate> pix_to_do =
            pixel_coordinates_above_threshold(to_do_image, 128.);

        if (pix_to_do.size() <= num_pixel/100)
            break;

        std::vector<PixelCoordinate> pix_to_do_actually = pix_to_do;
        while (pix_to_do_actually.size() < num_pixel/2) {
            pix_to_do_actually.insert(
                pix_to_do_actually.end(),
                pix_to_do.begin(),
                pix_to_do.end());
        }

        std::vector<Color> new_colors = acquire_pixels(
            world,
            visual_config,
            pix_to_do_actually);

        assign_pixel_colors_to_sum_and_exposure_image(
            pix_to_do_actually,
            new_colors,
            &sum_image,
            &exposure_image);

        image_from_sum_and_exposure(
            sum_image,
            exposure_image,
            image);

        previous_sobel_image = sobel_image;
        sobel_operator(*image, &sobel_image);
        truncate_to_255(&sobel_image);

        fabs_difference(
            previous_sobel_image,
            sobel_image,
            &diff_image);

        for (unsigned int pix = 0; pix < to_do_image.raw.size(); pix++) {
            to_do_image.raw.at(pix).r = 0.;
            to_do_image.raw.at(pix).g = 0.;
            to_do_image.raw.at(pix).b = 0.;
        }

        luminance_threshold_dilatation(
            diff_image,
            visual_config->snapshot.noise_level,
            &to_do_image);

        iteration += 1;
    }
}

Color do_one_ray(
    int id,
    const Frame* world,
    const Config* visual_config,
    random::Mt19937 *prng,
    const PixelCoordinate pixel,
    const ApertureCamera* cam) {
    (void)id;
    CameraRay cam_ray = cam->get_ray_for_pixel_in_row_and_col(
        pixel.row,
        pixel.col,
        prng);
    Tracer tracer(&cam_ray, world, visual_config, prng);
    return tracer.color;
}

std::vector<Color> ApertureCamera::acquire_pixels(
    const Frame* world,
    const Config* visual_config,
    const std::vector<PixelCoordinate> pixels
) {
    uint64_t num_threads = std::thread::hardware_concurrency();

    ctpl::thread_pool pool(num_threads);
    random::Mt19937 prng;
    std::vector<std::future<Color>> results(pixels.size());

    for (uint64_t j = 0; j < pixels.size(); ++j) {
        results[j] = pool.push(
            do_one_ray,
            world,
            visual_config,
            &prng,
            pixels[j],
            this);
    }

    std::vector<Color> out(pixels.size());
    for (uint64_t i = 0; i < pixels.size(); i ++) {
        out[i] = results[i].get();
    }
    return out;
}

}  // namespace visual
}  // namespace merlict
