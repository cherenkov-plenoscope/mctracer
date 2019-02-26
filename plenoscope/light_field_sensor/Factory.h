// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_LIGHTFIELDSENSOR_FACTORY_H_
#define PLENOSCOPE_LIGHTFIELDSENSOR_FACTORY_H_

#include <string>
#include "merlict/merlict.h"
#include "plenoscope/light_field_sensor/Geometry.h"
#include "plenoscope/PlenoscopeScenery.h"

namespace plenoscope {
namespace light_field_sensor {

class Factory {
    merlict::sensor::Sensors *sub_pixel_sensors;
 public:
    const Geometry* geometry;
    merlict::Scenery* scenery;
    explicit Factory(const Geometry* geo);
    void add_light_field_sensor_to_frame_in_scenery(
        merlict::Frame* frame,
        merlict::Scenery* scenery);
    void add_demonstration_light_field_sensor_to_frame_in_scenery(
        merlict::Frame *frame,
        merlict::Scenery* scenery);
    merlict::sensor::Sensors* get_sub_pixels()const;
 private:
    void add_lens_array(merlict::Frame* frame);
    void add_light_field_sensor_frontplate(merlict::Frame* frame);
    void add_image_sensor_housing(merlict::Frame* frame);
    void add_pixel_bin_array(merlict::Frame* frame);
    void add_pixel_bin_with_name_at_pos(
        merlict::Frame* frame,
        const std::string name,
        const merlict::Vec3 pos);
    void add_lixel_sensor_plane(merlict::Frame* frame);
};

}  // namespace light_field_sensor
}  // namespace plenoscope

#endif  // PLENOSCOPE_LIGHTFIELDSENSOR_FACTORY_H_
