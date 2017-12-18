// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERAMAN_FIELDOFVIEW_H_
#define VISUAL_CAMERAMAN_FIELDOFVIEW_H_

#include "Tools/Tools.h"
#include "Verbosity.h"

namespace Visual {
namespace CameraMan {

class FieldOfView: public Verbosity {
 public:
    explicit FieldOfView(CameraDevice* camera);
    void increase_when_possible();
    void decrease_when_possible();
    void set_default();
    double get_default_FoV_in_rad()const;
    double max_FoV_in_rad = Deg2Rad(175.0);
    double min_FoV_in_rad = Deg2Rad(0.0001);
    double default_FoV_in_rad = Deg2Rad(65.0);

 private:
    CameraDevice* camera;
    void increase();
    void decrease();
    double rate()const;
    bool too_large_when_increased()const;
    bool too_small_when_decreased()const;
    void print_manipulation_when_verbose()const;
    void print_can_not_be_decreased_when_verbose()const;
    void print_can_not_be_increased_when_verbose()const;
    void print_prefix()const;
};

}  // namespace CameraMan
}  // namespace Visual

#endif  // VISUAL_CAMERAMAN_FIELDOFVIEW_H_
