// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERAOPERATOR_FIELDOFVIEW_H_
#define VISUAL_CAMERAOPERATOR_FIELDOFVIEW_H_

#include "Core/tools.h"
#include "Verbosity.h"

namespace relleums {
namespace visual {
namespace camera_operator {

class FieldOfView: public Verbosity {
 public:
    explicit FieldOfView(CameraDevice* camera);
    void increase_when_possible();
    void decrease_when_possible();
    void set_default();
    double max_fov = deg2rad(175.0);
    double min_fov = deg2rad(0.0001);
    double default_fov = deg2rad(65.0);
    double rate = 1.1;

 private:
    CameraDevice* camera;
    void increase();
    void decrease();
    bool below_max_when_increased()const;
    bool above_min_when_decreased()const;
    void print_manipulation_when_verbose()const;
    void print_can_not_be_decreased_when_verbose()const;
    void print_can_not_be_increased_when_verbose()const;
    void print_prefix()const;
};

}  // namespace camera_operator
}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_CAMERAOPERATOR_FIELDOFVIEW_H_
