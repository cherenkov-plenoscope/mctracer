// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERARAY_H_
#define VISUAL_CAMERARAY_H_

namespace merlict {class Frame;}
namespace merlict {class Intersection;}

#include <string>
#include "merlict/Ray.h"
#include "merlict/Color.h"
#include "Config.h"

namespace merlict {
namespace visual {

class CameraRay :public Ray {
 protected:
    Color color;
 public:
    CameraRay();
    CameraRay(const Vec3 support, const Vec3 direction);
    std::string str()const;
};

}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_CAMERARAY_H_
