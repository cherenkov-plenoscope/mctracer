// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_LIGHTFIELDSENSOR_FACTORY_H_
#define PLENOSCOPE_LIGHTFIELDSENSOR_FACTORY_H_

#include <string>
#include "sensor/PhotonSensor.h"
#include "Plenoscope/LightFieldSensor/Geometry.h"
#include "Plenoscope/PlenoscopeScenery.h"
#include "Scenery/Scenery.h"

namespace Plenoscope {
namespace LightFieldSensor {

class Factory {
    relleums::sensor::Sensors *sub_pixel_sensors;
 public:
    const Geometry* geometry;
    relleums::Scenery* scenery;
    explicit Factory(const Geometry* geo);
    void add_light_field_sensor_to_frame_in_scenery(
        relleums::Frame* frame,
        relleums::Scenery* scenery);
    void add_demonstration_light_field_sensor_to_frame_in_scenery(
        relleums::Frame *frame,
        relleums::Scenery* scenery);
    relleums::sensor::Sensors* get_sub_pixels()const;
 private:
    void add_lens_array(relleums::Frame* frame);
    void add_light_field_sensor_frontplate(relleums::Frame* frame);
    void add_image_sensor_housing(relleums::Frame* frame);
    void add_pixel_bin_array(relleums::Frame* frame);
    void add_pixel_bin_with_name_at_pos(
        relleums::Frame* frame,
        const std::string name,
        const relleums::Vec3 pos);
    void add_lixel_sensor_plane(relleums::Frame* frame);
};

}  // namespace LightFieldSensor
}  // namespace Plenoscope

#endif  // PLENOSCOPE_LIGHTFIELDSENSOR_FACTORY_H_
