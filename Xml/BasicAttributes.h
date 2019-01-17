// Copyright 2014 Sebastian A. Mueller
#ifndef XML_BASICATTRIBUTES_H_
#define XML_BASICATTRIBUTES_H_

#include <string>
#include "Core/Vec3.h"

namespace Xml {
    std::string av(const std::string &att, const std::string &val);
    std::string comment(const std::string &text);
    std::string to_string(const Vec3 &vec);
    std::string to_string(const bool b);
}  // namespace Xml

#endif  // XML_BASICATTRIBUTES_H_
