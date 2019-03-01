// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/light_field_sensor/Factory.h"
#include <math.h>
#include <vector>
#include "merlict/merlict.h"
namespace ml = merlict;


namespace plenoscope {
namespace light_field_sensor {

Factory::Factory(const Geometry* geo):
    geometry(geo),
    scenery(&ml::Scenery::void_scenery)
{}

void Factory::add_lens_array(ml::Frame* frame) {
    scenery->colors.add("lens_white", ml::COLOR_WHITE);
    const ml::Color* white = scenery->colors.get("lens_white");

    ml::Frame* lens_array = frame->append<ml::Frame>();
    lens_array->set_name_pos_rot("lens_array", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    std::vector<ml::Vec3> pixel_positions = geometry->pixel_positions();

    for (unsigned int i = 0; i < pixel_positions.size(); i++) {
        ml::BiConvexLensHexBound* lens =
            lens_array->append<ml::BiConvexLensHexBound>();
        lens->set_name_pos_rot(
            "lens_" + std::to_string(i),
            pixel_positions.at(i),
            ml::ROT3_UNITY);
        lens->set_outer_color(white);
        lens->set_inner_color(white);
        lens->set_inner_refraction(geometry->config.lens_refraction);
        lens->set_curvature_radius_and_outer_hex_radius(
            geometry->pixel_lens_curvature_radius(),
            geometry->pixel_lens_outer_aperture_radius());
    }
}

void Factory::add_pixel_bin_array(ml::Frame* frame) {
    ml::Frame* bin_array = frame->append<ml::Frame>();
    bin_array->set_name_pos_rot(
        "bin_array",
        ml::Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()),
        ml::ROT3_UNITY);

    std::vector<ml::Vec3> flower_positions =
        geometry->paxel_grid_center_positions();
    scenery->colors.add("bin_wall_green", ml::COLOR_GREEN);

    for (unsigned int i = 0; i < flower_positions.size(); i++) {
        add_pixel_bin_with_name_at_pos(
            bin_array,
            "bin_" + std::to_string(i),
            flower_positions.at(i));
    }
}

void Factory::add_pixel_bin_with_name_at_pos(
    ml::Frame* frame,
    const std::string name,
    const ml::Vec3 pos
) {
    const ml::Color* green = scenery->colors.get("bin_wall_green");

    ml::Frame* bin = frame->append<ml::Frame>();
    bin->set_name_pos_rot(name, pos, ml::ROT3_UNITY);

    const double R = geometry->pixel_lens_inner_aperture_radius();
    const double hight = geometry->bin_hight();

    for (unsigned int i = 0; i < 6; i++) {
        const double phi = static_cast<double>(i)*1.0/3.0*M_PI;
        ml::Plane* binwall = bin->append<ml::Plane>();
        binwall->set_name_pos_rot(
            name + "_" + std::to_string(i),
            ml::Vec3(R*sin(phi), R*cos(phi), -0.5*hight),
            ml::Rot3(-M_PI*0.5, 0.0, phi));
        binwall->set_x_y_width(
            geometry->pixel_lens_outer_aperture_radius(),
            hight);
        binwall->set_outer_color(green);
        binwall->set_inner_color(green);
        binwall->set_outer_reflection(geometry->config.bin_reflection);
    }
}

void Factory::add_light_field_sensor_frontplate(ml::Frame* frame) {
    scenery->colors.add("light_field_sensor_housing_gray", ml::COLOR_GRAY);
    const ml::Color* gray = scenery->colors.get(
        "light_field_sensor_housing_gray");

    ml::HexGridAnnulus face_plate_grid(
        geometry->max_outer_sensor_radius() +
        geometry->pixel_spacing(),
        geometry->max_outer_sensor_radius() -
        geometry->pixel_lens_outer_aperture_radius(),
        geometry->pixel_spacing());

    std::vector<ml::Vec3> face_plate_positions = face_plate_grid.get_grid();
    ml::Frame* face_plate = frame->append<ml::Frame>();
    face_plate->set_name_pos_rot("face_plate", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    for (unsigned int i = 0; i < face_plate_positions.size(); i++) {
        ml::HexPlane* face = face_plate->append<ml::HexPlane>();
        face->set_name_pos_rot(
            "face_"+std::to_string(i),
            face_plate_positions.at(i),
            ml::ROT3_UNITY);
        face->set_outer_color(gray);
        face->set_inner_color(gray);
        face->set_outer_hex_radius(
            geometry->pixel_lens_outer_aperture_radius());
    }

    ml::Annulus* outer_front_ring = face_plate->append<ml::Annulus>();
    outer_front_ring->set_name_pos_rot(
        "outer_front_ring", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    outer_front_ring->set_outer_color(gray);
    outer_front_ring->set_inner_color(gray);
    outer_front_ring->set_outer_inner_radius(
        geometry->outer_sensor_housing_radius(),
        geometry->max_outer_sensor_radius());
}

void Factory::add_lixel_sensor_plane(ml::Frame* frame) {
    scenery->colors.add("light_field_sensor_cell_red", ml::COLOR_RED);
    const ml::Color* red = scenery->colors.get("light_field_sensor_cell_red");

    ml::Frame* sub_pixel_array = frame->append<ml::Frame>();
    sub_pixel_array->set_name_pos_rot(
        "lixel_array",
        ml::Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()),
        ml::ROT3_UNITY);

    const std::vector<ml::Vec3> &lixel_positions = geometry->lixel_positions();

    std::vector<ml::sensor::Sensor*> sub_pixels;
    sub_pixels.reserve(lixel_positions.size());

    for (unsigned int i = 0; i < lixel_positions.size(); i++) {
        ml::HexPlane* subpix = sub_pixel_array->append<ml::HexPlane>();
        subpix->set_name_pos_rot(
            "lixel_" + std::to_string(i),
            lixel_positions.at(i),
            ml::Rot3(0.0, 0.0, geometry->lixel_z_orientation()));
        subpix->set_outer_color(red);
        subpix->set_inner_color(red);
        subpix->set_outer_hex_radius(geometry->lixel_outer_radius());

        ml::sensor::Sensor* sub_pixel_sensor =
            new ml::sensor::Sensor(i, subpix);

        sub_pixels.push_back(sub_pixel_sensor);
    }

    sub_pixel_sensors = new ml::sensor::Sensors(sub_pixels);
}

void Factory::add_image_sensor_housing(ml::Frame *frame) {
    double housing_height = .67*geometry->outer_sensor_housing_radius();

    ml::Frame* sensor_housing = frame->append<ml::Frame>();
    sensor_housing->set_name_pos_rot(
        "sensor_housing", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::Disc* sensor_housing_top = sensor_housing->append<ml::Disc>();
    sensor_housing_top->set_name_pos_rot(
        "top",
        ml::Vec3(0.0, 0.0, housing_height),
        ml::ROT3_UNITY);
    sensor_housing_top->set_outer_color(&ml::COLOR_GRAY);
    sensor_housing_top->set_inner_color(&ml::COLOR_GRAY);
    sensor_housing_top->set_radius(geometry->outer_sensor_housing_radius());

    ml::Cylinder* sensor_housing_cylinder =
        sensor_housing->append<ml::Cylinder>();
    sensor_housing_cylinder->set_name_pos_rot(
        "cylinder",
        ml::VEC3_ORIGIN,
        ml::ROT3_UNITY);
    sensor_housing_cylinder->set_outer_color(&ml::COLOR_GRAY);
    sensor_housing_cylinder->set_inner_color(&ml::COLOR_GRAY);
    sensor_housing_cylinder->set_cylinder(
        geometry->outer_sensor_housing_radius(),
        ml::Vec3(0.0, 0.0, 0.0),
        ml::Vec3(0.0, 0.0, housing_height));
}

void Factory::add_light_field_sensor_to_frame_in_scenery(
    ml::Frame *frame,
    ml::Scenery* scenery
) {
    this->scenery = scenery;

    ml::Frame* light_field_sensor_front = frame->append<ml::Frame>();
    light_field_sensor_front->set_name_pos_rot(
        "front",
        ml::VEC3_ORIGIN,
        ml::ROT3_UNITY);

    add_lens_array(light_field_sensor_front);
    add_light_field_sensor_frontplate(light_field_sensor_front);

    add_image_sensor_housing(frame);
    add_pixel_bin_array(frame);
    add_lixel_sensor_plane(frame);
}

void Factory::add_demonstration_light_field_sensor_to_frame_in_scenery(
    ml::Frame *frame,
    ml::Scenery* scenery
) {
    this->scenery = scenery;

    // Add lens
    scenery->colors.add("lens_white", ml::COLOR_WHITE);
    const ml::Color* white = scenery->colors.get("lens_white");

    ml::BiConvexLensHexBound* lens = frame->append<ml::BiConvexLensHexBound>();
    lens->set_name_pos_rot("lens_0", ml::Vec3(0.0, 0.0, 0.0), ml::ROT3_UNITY);
    lens->set_outer_color(white);
    lens->set_inner_color(white);
    lens->set_inner_refraction(geometry->config.lens_refraction);
    lens->set_curvature_radius_and_outer_hex_radius(
        geometry->pixel_lens_curvature_radius(),
        geometry->pixel_lens_outer_aperture_radius());

    // Add bin walls
    ml::Frame* bin_array = frame->append<ml::Frame>();
    bin_array->set_name_pos_rot(
        "bin_array",
        ml::Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()),
        ml::ROT3_UNITY);

    scenery->colors.add("bin_wall_green", ml::COLOR_GREEN);

    add_pixel_bin_with_name_at_pos(
        bin_array,
        "bin_0",
        ml::Vec3(0.0, 0.0, 0.0));

    // Add lixels
    scenery->colors.add("light_field_sensor_cell_red", ml::COLOR_RED);
    const ml::Color* red = scenery->colors.get("light_field_sensor_cell_red");

    ml::Frame* sub_pixel_array = frame->append<ml::Frame>();
    sub_pixel_array->set_name_pos_rot(
        "lixel_array",
        ml::Vec3(0.0, 0.0, geometry->pixel_plane_to_paxel_plane_distance()),
        ml::ROT3_UNITY);

    const std::vector<ml::Vec3> lixel_positions =
        geometry->paxel_per_pixel_template_grid;

    std::vector<ml::sensor::Sensor*> sub_pixels;
    sub_pixels.reserve(geometry->paxel_per_pixel_template_grid.size());

    for (unsigned int i = 0; i < lixel_positions.size(); i++) {
        ml::HexPlane* subpix = sub_pixel_array->append<ml::HexPlane>();
        subpix->set_name_pos_rot(
            "lixel_" + std::to_string(i),
            lixel_positions.at(i),
            ml::Rot3(0.0, 0.0, geometry->lixel_z_orientation()));
        subpix->set_outer_color(red);
        subpix->set_inner_color(red);
        subpix->set_outer_hex_radius(0.95*geometry->lixel_outer_radius());

        ml::sensor::Sensor* sub_pixel_sensor =
            new ml::sensor::Sensor(i, subpix);

        sub_pixels.push_back(sub_pixel_sensor);
    }
    sub_pixel_sensors = new ml::sensor::Sensors(sub_pixels);
}

ml::sensor::Sensors* Factory::get_sub_pixels()const {
    return sub_pixel_sensors;
}

}  // namespace light_field_sensor
}  // namespace plenoscope
