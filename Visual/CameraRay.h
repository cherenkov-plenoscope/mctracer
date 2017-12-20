// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERARAY_H_
#define VISUAL_CAMERARAY_H_

class Frame;
class Intersection;

#include <string>
#include "Core/Ray.h"
#include "Core/Color.h"
#include "Config.h"

namespace Visual {

class CameraRay :public Ray {
 protected:
    Color color;
 public:
    CameraRay();
    CameraRay(const Vec3 support, const Vec3 direction);
    std::string str()const;
};

}  // namespace Visual

#endif  // VISUAL_CAMERARAY_H_
