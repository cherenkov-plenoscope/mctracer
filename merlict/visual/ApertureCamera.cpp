// Copyright 2014 Sebastian A. Mueller
#include "merlict/visual/ApertureCamera.h"
#include <vector>
#include <exception>
#include <future>
#include <thread>
#include "merlict/visual/Tracer.h"
#include "merlict/random/random.h"
#include "merlict/vitaliy_vitsentiy_thread_pool.h"


namespace merlict {
namespace visual {

void ApertureCamera::set_fStop_sesnorWidth(
    const double new_FStopNumber,
    const double new_SensorSizeX
) {
    set_F_stop_number(new_FStopNumber);
    set_sensor_size_using_width(new_SensorSizeX);
    update_sensor_pixel_pitch();

    set_default_object_distance();
    update_sensor_distance_given_focal_and_object_distance();
}

void ApertureCamera::set_F_stop_number(const double new_FStopNumber) {
    if (new_FStopNumber <= 0.0) {
        std::stringstream info;
        info << "Expected F-Stop number to be greater zero, but actual it is ";
        info << new_FStopNumber << ".";
        throw std::invalid_argument(info.str());
    }
    FStopNumber = new_FStopNumber;
}

void ApertureCamera::set_sensor_size_using_width(const double width_in_m) {
    if (width_in_m <= 0.0) {
        std::stringstream info;
        info << "Expected image sensor width to be greater zero, ";
        info << "but actual it is ";
        info << width_in_m << ".";
        throw std::invalid_argument(info.str());
    }
    const double width_to_height_ratio =
        static_cast<double>(num_cols)/
        static_cast<double>(num_rows);
    sensor_width_in_m = width_in_m;
    sensor_height_in_m = sensor_width_in_m / width_to_height_ratio;
}

void ApertureCamera::update_sensor_pixel_pitch() {
    PixelPitch_in_m = sensor_width_in_m/
        static_cast<double>(num_cols);
}

void ApertureCamera::set_default_object_distance() {
    set_object_distance(default_object_distance_in_m);
}

void ApertureCamera::set_object_distance(const double ObjectDistance_in_m) {
    if (ObjectDistance_in_m <= 0.0) {
        std::stringstream info;
        info << "Expected object distance to focus on to be ";
        info << "greater zero, but actual it is ";
        info << ObjectDistance_in_m << ".";
        throw std::invalid_argument(info.str());
    }
    this->ObjectDistance_in_m = ObjectDistance_in_m;
}

void ApertureCamera::update_sensor_distance_given_focal_and_object_distance() {
    // 1/f = 1/b + 1/g
    // f = FocalLength_in_m
    // b = SensorDistance_in_m
    // g = ObjectDistance_in_m
    SensorDistance_in_m = 1.0/(1.0/FocalLength_in_m - 1.0/ObjectDistance_in_m);
}

void ApertureCamera::set_focus_to(const double ObjectDistance_in_m) {
    set_object_distance(ObjectDistance_in_m);
    update_sensor_distance_given_focal_and_object_distance();
}

void ApertureCamera::set_field_of_view(const double field_of_view) {
    assert_field_of_view_is_valid(field_of_view);
    __field_of_view = field_of_view;
    update_focal_length();
}

void ApertureCamera::update_focal_length() {
    FocalLength_in_m = (sensor_width_in_m/2.0)/tan(__field_of_view/2.0);
    update_aperture_radius();
    update_sensor_distance_given_focal_and_object_distance();
}

void ApertureCamera::update_aperture_radius() {
    ApertureRadius_in_m = FocalLength_in_m/(2.0*FStopNumber);
}

std::string ApertureCamera::get_aperture_camera_print()const {
    std::stringstream out;
    out << "| Focal ratio      : " << FStopNumber << "\n";
    out << "| Focal length     : " << FocalLength_in_m*1e3 << " mm\n";
    out << "| Aperture diameter: " << ApertureRadius_in_m*2e3 << " mm\n";
    out << "| Sensor size      : ";
    out << 1e3*sensor_width_in_m << " x ";
    out << 1e3*sensor_height_in_m << " mm^2\n";
    out << "| Pixel pitch      : " << PixelPitch_in_m*1e6 << " um\n";
    out << "| Object distance  : " << ObjectDistance_in_m << " m\n";
    out << "| Sensor distance  : " << SensorDistance_in_m*1e3 << " mm\n";
    return out.str();
}

std::string ApertureCamera::str()const {
    std::stringstream out;
    out << camera_str() << get_aperture_camera_print();
    return out.str();
}

Vec3 ApertureCamera::get_random_point_on_bounded_aperture_plane(
    random::Mt19937 *prng
)const {
    Vec3 ap = prng->get_point_on_xy_disc_within_radius(ApertureRadius_in_m);
    return Vec3(ap.x, ap.y, SensorDistance_in_m);
}

Vec3 ApertureCamera::get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(
    const unsigned int row,
    const unsigned int col,
    random::Mt19937 *prng
)const {
    const int x_pos_on_sensor_in_pixel =  row - num_rows/2;
    const int y_pos_on_sensor_in_pixel =  col - num_cols/2;

    const double image_size_x = x_pos_on_sensor_in_pixel * PixelPitch_in_m;
    const double image_size_y = y_pos_on_sensor_in_pixel * PixelPitch_in_m;

    const double sensor_extension_x = (prng->uniform() - .5) * PixelPitch_in_m;
    const double sensor_extension_y = (prng->uniform() - .5) * PixelPitch_in_m;

    return Vec3(
        get_object_size_for_image_size(image_size_x + sensor_extension_x),
        get_object_size_for_image_size(image_size_y + sensor_extension_y),
        ObjectDistance_in_m);
}

double ApertureCamera::get_object_size_for_image_size(
    const double image_size_in_m
)const {
    return image_size_in_m * ObjectDistance_in_m / SensorDistance_in_m;
}

CameraRay ApertureCamera::get_ray_for_pixel_in_row_and_col(
    const unsigned int row,
    const unsigned int col,
    random::Mt19937 *prng
)const {
    const Vec3 support_vec_in_cam_frame =
        get_random_point_on_bounded_aperture_plane(prng);
    const Vec3 obj_plane_intersec_in_cam_frame =
        get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(row, col, prng);
    const Vec3 direction_vec_in_cam_frame =
        obj_plane_intersec_in_cam_frame - support_vec_in_cam_frame;
    return CameraRay(
        camera_ray_support_vector_in_world_frame(support_vec_in_cam_frame),
        camera_ray_direction_vector_in_world_frame(direction_vec_in_cam_frame));
}

Vec3 ApertureCamera::camera_ray_support_vector_in_world_frame(
    const Vec3 &cam_ray_support_in_cam_frame
)const {
    return __position + __camera2root.get_transformed_orientation(
        cam_ray_support_in_cam_frame);
}

Vec3 ApertureCamera::camera_ray_direction_vector_in_world_frame(
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
