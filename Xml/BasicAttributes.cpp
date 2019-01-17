// Copyright 2014 Sebastian A. Mueller
#include "Xml/BasicAttributes.h"
#include <sstream>
using std::string;

namespace Xml {

string av(const string &att, const string &val) {
    std::stringstream out;
    out << att << "=\"" << val << "\"";
    return out.str();
}

string comment(const string &text) {
    std::stringstream out;
    out << "<!-- " << text << " -->";
    return out.str();
}

string to_string(const Vec3 &vec) {
    std::stringstream out;
    out << "[" << vec.x << "," << vec.y << "," <<  vec.z << "]";
    return out.str();
}

string to_string(const bool b) {
    return (b? "true" : "false");
}

}  // namespace Xml
