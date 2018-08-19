// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERAOPERATOR_VERBOSITY_H_
#define VISUAL_CAMERAOPERATOR_VERBOSITY_H_

#include "Visual/CameraDevice.h"

namespace Visual {
namespace CameraOperator {

class Verbosity {
 protected:
    bool verbose = false;

 public:
    void set_verbosity(const bool verbosity);
};

}  // namespace CameraOperator
}  // namespace Visual

#endif  // VISUAL_CAMERAOPERATOR_VERBOSITY_H_
