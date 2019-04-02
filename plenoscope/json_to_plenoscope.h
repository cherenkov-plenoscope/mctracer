// Copyright 2018 Sebastian A. Mueller
#ifndef PLENOSCOPE_JSON_TO_PLENOSCOPE_H_
#define PLENOSCOPE_JSON_TO_PLENOSCOPE_H_

#include "plenoscope/PlenoscopeScenery.h"
#include "merlict_json/json.h"
#include "merlict/merlict.h"

namespace plenoscope {
namespace json {

merlict::Frame* add_light_field_sensor(
    merlict::Frame* mother,
    PlenoscopeScenery* scenery,
    const merlict::json::Object &o
) {
    merlict::Frame* light_field_sensor = mother->add<merlict::Frame>();
    merlict::json::set_frame(light_field_sensor, o);
    light_field_sensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        o.f8("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        o.f8("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter = merlict::deg2rad(o.f8("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat = merlict::deg2rad(o.f8("hex_pixel_FoV_flat2flat_deg"));
    config.num_paxel_on_pixel_diagonal =
        o.u8("num_paxel_on_pixel_diagonal");
    config.housing_overhead = o.f8("housing_overhead");
    config.lens_refraction = scenery->functions.get(
            o.st("lens_refraction_vs_wavelength"));
    config.bin_reflection = scenery->functions.get(
            o.st("bin_reflection_vs_wavelength"));

    PlenoscopeInScenery pis(config);
    light_field_sensor::Factory lfs_factory(
        &pis.light_field_sensor_geometry);
    lfs_factory.add_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);
    pis.frame = light_field_sensor;
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    scenery->plenoscopes.push_back(pis);
    return light_field_sensor;
}

merlict::Frame* add_light_field_sensor_demonstration(
    merlict::Frame* mother,
    PlenoscopeScenery* scenery,
    const merlict::json::Object &o
) {
    merlict::Frame* light_field_sensor = mother->add<merlict::Frame>();
    merlict::json::set_frame(light_field_sensor, o);
    light_field_sensor::Config config;
    config.sensor_plane2imaging_system = *light_field_sensor->frame2mother();
    config.expected_imaging_system_focal_length =
        o.f8("expected_imaging_system_focal_length");
    config.expected_imaging_system_max_aperture_radius =
        o.f8("expected_imaging_system_aperture_radius");
    config.max_FoV_diameter = merlict::deg2rad(o.f8("max_FoV_diameter_deg"));
    config.pixel_FoV_hex_flat2flat = merlict::deg2rad(o.f8("hex_pixel_FoV_flat2flat_deg"));
    config.num_paxel_on_pixel_diagonal =
        o.u8("num_paxel_on_pixel_diagonal");
    config.housing_overhead = o.f8("housing_overhead");
    config.lens_refraction = scenery->functions.get(
            o.st("lens_refraction_vs_wavelength"));
    config.bin_reflection = scenery->functions.get(
            o.st("bin_reflection_vs_wavelength"));

    PlenoscopeInScenery pis(config);
    light_field_sensor::Factory lfs_factory(
        &pis.light_field_sensor_geometry);
    lfs_factory.add_demonstration_light_field_sensor_to_frame_in_scenery(
        light_field_sensor,
        scenery);
    pis.frame = light_field_sensor;
    pis.light_field_channels = lfs_factory.get_sub_pixels();
    scenery->plenoscopes.push_back(pis);
    return light_field_sensor;
}

void make_children(
    merlict::Frame* mother,
    PlenoscopeScenery* scenery,
    const merlict::json::Object &o
) {
    std::map<std::string, merlict::json::json_to_frame> j2s = merlict::json::basic_scenery();
    const uint64_t num_children = o.size();
    for (uint64_t cidx = 0; cidx < num_children; cidx++) {
        const merlict::json::Object &jchild = o.obj(cidx);
        std::string type = jchild.st("type");
        if (merlict::json::has_key(j2s, type)) {
            merlict::json::json_to_frame us = merlict::json::get(j2s, type);
            make_children(
                us(mother, scenery, jchild),
                scenery,
                jchild.obj("children"));
        } else if (merlict::txt::is_equal(type, "LightFieldSensor")) {
            make_children(
                add_light_field_sensor(mother, scenery, jchild),
                scenery,
                jchild.obj("children"));
        } else if (merlict::txt::is_equal(type, "LightFieldSensor_for_demonstration")) {
            make_children(
                add_light_field_sensor_demonstration(mother, scenery, jchild),
                scenery,
                jchild.obj("children"));
        }
    }
}

void append_to_frame_in_scenery_from_json_obj(
    merlict::Frame* mother,
    PlenoscopeScenery* scenery,
    const merlict::json::Object &o
) {
    merlict::json::add_functions(&scenery->functions, o.obj("functions"));
    merlict::json::add_colors(&scenery->colors, o.obj("colors"));
    make_children(mother, scenery, o.obj("children"));
}

void append_to_frame_in_scenery(
    merlict::Frame* mother,
    PlenoscopeScenery* scenery,
    const std::string &path) {
    append_to_frame_in_scenery_from_json_obj(
        mother,
        scenery,
        merlict::json::load(path));
}

}  // namespace json
}  // namespace plenoscope

#endif  // PLENOSCOPE_JSON_TO_PLENOSCOPE_H_
