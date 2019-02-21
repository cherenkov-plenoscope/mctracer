// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERAOPERATOR_TRANSLATION_H_
#define VISUAL_CAMERAOPERATOR_TRANSLATION_H_

#include <string>
#include "Verbosity.h"

namespace relleums {
namespace visual {
namespace camera_operator {

class Translation: public Verbosity {
 public:
    explicit Translation(CameraDevice* camera_to_work_with);
    void set_default_position();
    void set_default_position(const Vec3 default_pos);
    void move_forward();
    void move_back();
    void move_left();
    void move_right();
    void move_right(const double step_in_m);
    void move_up();
    void move_down();
    void move_to(const Vec3 pos);
 private:
    CameraDevice* camera;
    bool verbosity = false;
    Vec3 default_position = Vec3::ORIGIN;

    void print_camera_moved_in_direction(const std::string dir)const;
    double translation_increment()const;
};

}  // namespace camera_operator
}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_CAMERAOPERATOR_TRANSLATION_H_
