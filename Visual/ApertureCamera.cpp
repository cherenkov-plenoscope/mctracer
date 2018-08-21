// Copyright 2014 Sebastian A. Mueller
#include "Visual/ApertureCamera.h"
#include <vector>
#include <exception>
#include "Visual/Tracer.h"
#include "Core/Random/Random.h"
using std::string;
using std::stringstream;

namespace Visual {
/*
struct Imagef {
    unsigned int number_cols;
    unsigned int number_rows;
    std::vector<Color> raw;
    Imagef(unsigned int _number_cols, unsigned int _number_rows):
        number_cols(_number_cols),
        number_rows(_number_rows),
        raw(std::vector<Color>(number_cols*number_rows)) {}
    Color at_col_row(unsigned int col, unsigned int row)const {
        return raw.at(_idx(col, row));
    }
    void set_col_row(unsigned int col, unsigned int row, Color c) {
        raw.at(_idx(col, row)) = c;
    }
    unsigned int _idx(unsigned int col, unsigned int row)const {
        return col*number_rows + row;
    }
};

Imagef truncate_to_255(const Imagef &image) {
    Imagef rc(image.number_cols, image.number_rows);
    for (unsigned int col = 0; col < image.number_cols; col++) {
        for (unsigned int row = 0; row < image.number_rows; row++) {
            Color c = image.at_col_row(col, row);
            if (c.r > 255.) c.r = 255.;
            if (c.g > 255.) c.g = 255.;
            if (c.b > 255.) c.b = 255.;
            rc.set_col_row(col, row, c);
        }
    }
    return rc;
}

Image to_255_image(const Imagef &image) {
    Imagef trunc_image = truncate_to_255(image);
    Image rc(image.number_cols, image.number_rows);
    for (unsigned int col = 0; col < image.number_cols; col++) {
        for (unsigned int row = 0; row < image.number_rows; row++) {
            Color c(
                static_cast<unsigned char>(trunc_image.at_col_row(col, row).r),
                static_cast<unsigned char>(trunc_image.at_col_row(col, row).g),
                static_cast<unsigned char>(trunc_image.at_col_row(col, row).b));
            rc.set_row_col_to_color(row, col, c);
        }
    }
    return rc;
}

Imagef sobel_operator(const Imagef &image) {
    Imagef rc(image.number_cols, image.number_rows);
    for (unsigned int col = 1; col < image.number_cols - 1; col++) {
        for (unsigned int row = 1; row < image.number_rows - 1; row++) {
            double xr = 0;
            double xg = 0;
            double xb = 0;

            Color z;
            z = image.at_col_row(col - 1, row + 1);
            xr += -1*z.r; xg += -1*z.g; xb += -1*z.b;
            z = image.at_col_row(col - 1, row);
            xr += -2*z.r; xg += -2*z.g; xb += -2*z.b;
            z = image.at_col_row(col - 1, row - 1);
            xr += -1*z.r; xg += -1*z.g; xb += -1*z.b;

            z = image.at_col_row(col + 1, row + 1);
            xr += z.r; xg += z.g; xb += z.b;
            z = image.at_col_row(col + 1, row);
            xr += 2*z.r; xg += 2*z.g; xb += 2*z.b;
            z = image.at_col_row(col + 1, row - 1);
            xr += z.r; xg += z.g; xb += z.b;

            double yr = 0;
            double yg = 0;
            double yb = 0;
            z = image.at_col_row(col + 1, row + 1);
            yr += -1*z.r; yg += -1*z.g; yb += -1*z.b;
            z = image.at_col_row(col    , row + 1);
            yr += -2*z.r; yg += -2*z.g; yb += -2*z.b;
            z = image.at_col_row(col - 1, row + 1);
            yr += -1*z.r; yg += -1*z.g; yb += -1*z.b;

            z = image.at_col_row(col + 1, row - 1);
            yr += z.r; yg += z.g; yb += z.b;
            z = image.at_col_row(col    , row - 1);
            yr += 2*z.r; yg += 2*z.g; yb += 2*z.b;
            z = image.at_col_row(col - 1, row - 1);
            yr += z.r; yg += z.g; yb += z.b;

            double gr = sqrt(xr*xr + yr*yr);
            double gg = sqrt(xg*xg + yg*yg);
            double gb = sqrt(xb*xb + yb*yb);
            Color g(gr, gg, gb);
            rc.set_col_row(col, row, g);
        }
    }
    return rc;
}

void assign_pixel_colors_to_sum_and_exposure_image(
    const std::vector<PixelCoordinate> &pixels,
    const std::vector<Color> &colors,
    Imagef *sum_image,
    Imagef* exposure_image
) {
    for (unsigned int pix = 0; pix < pixels.size(); pix++) {
        const unsigned int row = pixels.at(pix).row;
        const unsigned int col = pixels.at(pix).col;
        Color c;
        c.r = sum_image->at_col_row(col, row).r + colors.at(pix).r;
        c.g = sum_image->at_col_row(col, row).g + colors.at(pix).g;
        c.b = sum_image->at_col_row(col, row).b + colors.at(pix).b;
        sum_image->set_col_row(col, row, c);
        Color e;
        e.r = exposure_image->at_col_row(col, row).r + 1.;
        e.g = exposure_image->at_col_row(col, row).g + 1.;
        e.b = exposure_image->at_col_row(col, row).b + 1.;
        exposure_image->set_col_row(col, row, e);
    }
}


Imagef luminance_threshold_dilatation(
    const Imagef &image,
    const float threshold
) {
    const Color o(255., 255., 255.);
    const int rows = image.number_rows;
    const int cols = image.number_cols;
    Imagef rc(cols, rows);
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            double luminance = 0.0;
            Color c = image.at_col_row(col, row);
            luminance = c.r + c.g + c.b;
            if (luminance > threshold) {
                for (int orow = -1; orow < 2; orow++) {
                    for (int ocol = -1; ocol < 2; ocol++) {
                        if (
                            row + orow >= 0 &&
                            col + ocol >= 0 &&
                            row + orow < rows &&
                            col + ocol < cols) {
                            rc.set_col_row(col + ocol, row + orow, o);
                        }
                    }
                }
            }
        }
    }
    return rc;
}


Imagef image_from_sum_and_exposure(
    const Imagef &sum,
    const Imagef &exposure
) {
    Imagef image(sum.number_cols, sum.number_rows);
    for (unsigned int col = 0; col < sum.number_cols; col++) {
        for (unsigned int row = 0; row < sum.number_rows; row++) {
            Color c;
            c.r = sum.at_col_row(col, row).r/exposure.at_col_row(col, row).r;
            c.g = sum.at_col_row(col, row).g/exposure.at_col_row(col, row).g;
            c.b = sum.at_col_row(col, row).b/exposure.at_col_row(col, row).b;
            image.set_col_row(col, row, c);
        }
    }
    return image;
}

std::vector<PixelCoordinate> pixel_coordinates_above_threshold(
    const Imagef &image,
    const double threshold
) {
    std::vector<PixelCoordinate> coordinates;
    for (unsigned int row = 0; row < image.number_rows; row++) {
        for (unsigned int col = 0; col < image.number_cols; col++) {
            double lum = 0.0;
            Color c = image.at_col_row(col, row);
            lum = c.r + c.g + c.b;
            if (lum > threshold) {
                PixelCoordinate pc;
                pc.row = row;
                pc.col = col;
                coordinates.push_back(pc);
            }
        }
    }
    return coordinates;
}

Imagef fabs_image(const Imagef &a, const Imagef &b) {
    Imagef rc(a.number_cols, a.number_rows);
    for (unsigned int row = 0; row < a.number_rows; row++) {
        for (unsigned int col = 0; col < a.number_cols; col++) {
            Color c(
                fabs(a.at_col_row(col, row).r - b.at_col_row(col, row).r),
                fabs(a.at_col_row(col, row).g - b.at_col_row(col, row).g),
                fabs(a.at_col_row(col, row).b - b.at_col_row(col, row).b));
            rc.set_col_row(col, row, c);
        }
    }
    return rc;
}
*/

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
        stringstream info;
        info << "Expected F-Stop number to be greater zero, but actual it is ";
        info << new_FStopNumber << ".";
        throw std::invalid_argument(info.str());
    }
    FStopNumber = new_FStopNumber;
}

void ApertureCamera::set_sensor_size_using_width(const double width_in_m) {
    if (width_in_m <= 0.0) {
        stringstream info;
        info << "Expected image sensor width to be greater zero, ";
        info << "but actual it is ";
        info << width_in_m << ".";
        throw std::invalid_argument(info.str());
    }
    const double width_to_height_ratio =
        static_cast<double>(image.number_cols)/
        static_cast<double>(image.number_rows);
    sensor_width_in_m = width_in_m;
    sensor_height_in_m = sensor_width_in_m / width_to_height_ratio;
}

void ApertureCamera::update_sensor_pixel_pitch() {
    PixelPitch_in_m = sensor_width_in_m/
        static_cast<double>(image.number_cols);
}

void ApertureCamera::set_default_object_distance() {
    set_object_distance(default_object_distance_in_m);
}

void ApertureCamera::set_object_distance(const double ObjectDistance_in_m) {
    if (ObjectDistance_in_m <= 0.0) {
        stringstream info;
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

void ApertureCamera::set_FoV_in_rad(const double field_of_view) {
    assert_field_of_view_is_valid(field_of_view);
    this -> field_of_view = field_of_view;
    update_focal_length();
}

void ApertureCamera::update_focal_length() {
    FocalLength_in_m = (sensor_width_in_m/2.0)/tan(field_of_view/2.0);
    update_aperture_radius();
    update_sensor_distance_given_focal_and_object_distance();
}

void ApertureCamera::update_aperture_radius() {
    ApertureRadius_in_m = FocalLength_in_m/(2.0*FStopNumber);
}

void ApertureCamera::auto_focus(const Frame* world) {
    set_focus_to(get_average_object_distance(world));
}

unsigned int ApertureCamera::_5_permil_of_pixels()const {
    return (image.number_cols*image.number_rows)*5e-4;
}

double ApertureCamera::get_average_object_distance(const Frame* world) {
    double sum_of_valid_object_distances = 0.0;
    unsigned int number_of_valid_distances = 0;

    for (
        unsigned int pixel_it = 0;
        pixel_it < _5_permil_of_pixels();
        pixel_it++
    ) {
        CameraRay ray = get_ray_for_pixel_in_row_and_col(
            get_random_row(),
            get_random_col());

        DistanceMeter dist_meter(&ray, world);

        if (dist_meter.faces_an_object) {
            number_of_valid_distances++;
            sum_of_valid_object_distances = sum_of_valid_object_distances +
                dist_meter.distance_to_closest_object;
        }
    }

    if (number_of_valid_distances == 0)
        return default_object_distance_in_m;
    else
        return sum_of_valid_object_distances/
            static_cast<double>(number_of_valid_distances);
}

unsigned int ApertureCamera::get_random_row() {
    return (unsigned int)(floor(prng.uniform()*image.number_rows));
}

unsigned int ApertureCamera::get_random_col() {
    return (unsigned int)(floor(prng.uniform()*image.number_cols));
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
    out << get_camera_print() << get_aperture_camera_print();
    return out.str();
}

Vec3 ApertureCamera::get_random_point_on_bounded_aperture_plane() {
    Vec3 ap = prng.get_point_on_xy_disc_within_radius(ApertureRadius_in_m);
    return Vec3(ap.x, ap.y, SensorDistance_in_m);
}

Vec3 ApertureCamera::get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(
    const unsigned int row, const unsigned int col
) {
    const int x_pos_on_sensor_in_pixel =  row - image.number_rows/2;
    const int y_pos_on_sensor_in_pixel =  col - image.number_cols/2;

    const double image_size_x = x_pos_on_sensor_in_pixel * PixelPitch_in_m;
    const double image_size_y = y_pos_on_sensor_in_pixel * PixelPitch_in_m;

    return Vec3(
        get_object_size_for_image_size(image_size_x),
        get_object_size_for_image_size(image_size_y),
        ObjectDistance_in_m);
}

double ApertureCamera::get_object_size_for_image_size(
    const double image_size_in_m
)const {
    return image_size_in_m * ObjectDistance_in_m / SensorDistance_in_m;
}

CameraRay ApertureCamera::get_ray_for_pixel_in_row_and_col(
    const unsigned int row, const unsigned int col
) {
    const Vec3 support_vec_in_cam_frame =
        get_random_point_on_bounded_aperture_plane();
    const Vec3 obj_plane_intersec_in_cam_frame =
        get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(row, col);
    const Vec3 direction_vec_in_cam_frame =
        obj_plane_intersec_in_cam_frame - support_vec_in_cam_frame;
    return CameraRay(
        camera_ray_support_vector_in_world_frame(support_vec_in_cam_frame),
        camera_ray_direction_vector_in_world_frame(direction_vec_in_cam_frame));
}

Vec3 ApertureCamera::camera_ray_support_vector_in_world_frame(
    const Vec3 &cam_ray_support_in_cam_frame
)const {
    return pos_in_root + camera2root.get_transformed_orientation(
        cam_ray_support_in_cam_frame);
}

Vec3 ApertureCamera::camera_ray_direction_vector_in_world_frame(
    const Vec3 &cam_ray_direction
)const {
    return camera2root.get_transformed_orientation(cam_ray_direction);
}

void ApertureCamera::acquire_image(
    const Frame* world,
    const Config* visual_config
) {
    unsigned int rows = image.number_rows;
    unsigned int cols = image.number_cols;

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

    Image reconstructed_image = image_from_sum_and_exposure(
        sum_image,
        exposure_image);

    Image sobel_image = sobel_operator(reconstructed_image);
    sobel_image = truncate_to_255(sobel_image);

    Image to_do_image = luminance_threshold_dilatation(sobel_image, 128.);

    const unsigned int MAX_ITERATIONS = 100;
    const unsigned int MIN_NUMBER_RAYS = 1000;
    unsigned int iteration = 0;
    Image diff_image(cols, rows);
    Image previous_sobel_image(cols, rows);

    while (true) {
        if (iteration >= MAX_ITERATIONS)
            break;

        std::vector<PixelCoordinate> pix_to_do =
            pixel_coordinates_above_threshold(to_do_image, 128.);

        if (pix_to_do.size() <= MIN_NUMBER_RAYS)
            break;

        std::vector<Color> new_colors = acquire_pixels(
            world,
            visual_config,
            pix_to_do);

        assign_pixel_colors_to_sum_and_exposure_image(
            pix_to_do,
            new_colors,
            &sum_image,
            &exposure_image);

        reconstructed_image = image_from_sum_and_exposure(
            sum_image,
            exposure_image);

        previous_sobel_image  = sobel_image;
        sobel_image = sobel_operator(reconstructed_image);
        sobel_image = truncate_to_255(sobel_image);

        diff_image = fabs_image(
            previous_sobel_image,
            sobel_image);

        diff_image = luminance_threshold_dilatation(
            diff_image,
            visual_config->snapshot.noise_level);

        to_do_image = luminance_threshold_dilatation(diff_image, 128.);

        iteration += 1;
    }

    image = reconstructed_image;
}

std::vector<Color> ApertureCamera::acquire_pixels(
    const Frame* world,
    const Config* visual_config,
    const std::vector<PixelCoordinate> pixels
) {
    int HadCatch = 0;
    Random::Mt19937 prng;
    std::vector<Color> out(pixels.size());
    #pragma omp parallel shared(visual_config, world, HadCatch)
    {
        #pragma omp for schedule(dynamic)
        for (unsigned int p = 0; p < pixels.size(); p++) {
            try {
                CameraRay cam_ray = get_ray_for_pixel_in_row_and_col(
                    pixels[p].row,
                    pixels[p].col);
                Tracer tracer(&cam_ray, world, visual_config, &prng);
                out[p] = tracer.color;
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
        info << "ApertureCamera::" << __func__ << "() in " << __FILE__ << ", ";
        info << __LINE__ << "\n";
        info << "Cought exception during multithread ray tracing.\n";
        throw std::runtime_error(info.str());
    }
    return out;
}

}  // namespace Visual
