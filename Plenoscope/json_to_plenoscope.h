// Copyright 2018 Sebastian A. Mueller
#ifndef PLENOSCOPE_JSON_TO_PLENOSCOPE_H_
#define PLENOSCOPE_JSON_TO_PLENOSCOPE_H_

#include "Plenoscope/PlenoscopeScenery.h"
#include "./json.h"
#include "Tools/StringTools.h"

namespace Plenoscope {
namespace json {

relleums::Frame* add_light_field_sensor(
    relleums::Frame* mother,
    PlenoscopeScenery* scenery,
    const relleums::json::Object &o
) {
    relleums::Frame* light_field_sensor = mother->append<relleums::Frame>();
    relleums::json::set_frame(light_field_sensor, o);
    LightFieldSensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        o.f8("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        o.f8("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter = relleums::Deg2Rad(o.f8("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat = relleums::Deg2Rad(o.f8("hex_pixel_FoV_flat2flat_deg"));
    config.number_of_paxel_on_pixel_diagonal =
        o.u8("number_of_paxel_on_pixel_diagonal");
    config.housing_overhead = o.f8("housing_overhead");
    config.lens_refraction = scenery->functions.get(
            o.st("lens_refraction_vs_wavelength"));
    config.bin_reflection = scenery->functions.get(
            o.st("bin_reflection_vs_wavelength"));

    PlenoscopeInScenery pis(config);
    LightFieldSensor::Factory lfs_factory(
        &pis.light_field_sensor_geometry);
    lfs_factory.add_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);
    pis.frame = light_field_sensor;
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    scenery->plenoscopes.push_back(pis);
    return light_field_sensor;
}

relleums::Frame* add_light_field_sensor_demonstration(
    relleums::Frame* mother,
    PlenoscopeScenery* scenery,
    const relleums::json::Object &o
) {
    relleums::Frame* light_field_sensor = mother->append<relleums::Frame>();
    relleums::json::set_frame(light_field_sensor, o);
    LightFieldSensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        o.f8("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        o.f8("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter = relleums::Deg2Rad(o.f8("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat = relleums::Deg2Rad(o.f8("hex_pixel_FoV_flat2flat_deg"));
    config.number_of_paxel_on_pixel_diagonal =
        o.u8("number_of_paxel_on_pixel_diagonal");
    config.housing_overhead = o.f8("housing_overhead");
    config.lens_refraction = scenery->functions.get(
            o.st("lens_refraction_vs_wavelength"));
    config.bin_reflection = scenery->functions.get(
            o.st("bin_reflection_vs_wavelength"));

    PlenoscopeInScenery pis(config);
    LightFieldSensor::Factory lfs_factory(
        &pis.light_field_sensor_geometry);
    lfs_factory.add_demonstration_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);
    pis.frame = light_field_sensor;
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    scenery->plenoscopes.push_back(pis);
    return light_field_sensor;
}

void append_to_frame_in_scenery(
    relleums::Frame* mother,
    PlenoscopeScenery* scenery,
    const std::string &path) {
    append_to_frame_in_scenery(mother, scenery, relleums::json::load(path));
}

void append_to_frame_in_scenery(
    relleums::Frame* mother,
    PlenoscopeScenery* scenery,
    const relleums::json::Object &o
) {
    relleums::json::add_functions(&scenery->functions, o.obj("functions"));
    relleums::json::add_colors(&scenery->colors, o.obj("colors"));
    make_children(mother, scenery, o.obj("children"));
}

void make_children(
    relleums::Frame* mother,
    PlenoscopeScenery* scenery,
    const relleums::json::Object &o
) {
    std::map<std::string, relleums::json::json_to_frame> j2s = relleums::json::basic_scenery();
    const uint64_t num_children = o.size();
    for (uint64_t cidx = 0; cidx < num_children; cidx++) {
        const relleums::json::Object &jchild = o.obj(cidx);
        std::string type = jchild.st("type");
        if (relleums::json::has_key(j2s, type)) {
            relleums::json::json_to_frame us = relleums::json::get(j2s, type);
            make_children(
                us(mother, scenery, jchild),
                scenery,
                jchild.obj("children"));
        } else if (relleums::StringTools::is_equal(type, "LightFieldSensor")) {
            make_children(
                add_light_field_sensor(mother, scenery, jchild),
                scenery,
                jchild.obj("children"));
        } else if (relleums::StringTools::is_equal(type, "LightFieldSensor_for_demonstration")) {
            make_children(
                add_light_field_sensor_demonstration(mother, scenery, jchild),
                scenery,
                jchild.obj("children"));
        }
    }
}

}  // namespace json
}  // namespace Plenoscope

#endif  // PLENOSCOPE_JSON_TO_PLENOSCOPE_H_