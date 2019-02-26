// Copyright 2014 Sebastian A. Mueller
#include "merlict/Color.h"
#include <math.h>
#include <sstream>
#include <exception>

namespace merlict {

Color::Color(const std::vector<Color> &colors) {
    float _r = 0.0;
    float _g = 0.0;
    float _b = 0.0;

    for (Color c : colors) {
        _r += c.r;
        _g += c.g;
        _b += c.b;
    }
    const float weight = 1.0/colors.size();

    r = _r*weight;
    g = _g*weight;
    b = _b*weight;
}

std::string Color::str()const {
    std::stringstream out;
    out << "(" << round(r);
    out << " " << round(g);
    out << " " << round(b) << ")";
    out << "RGB";
    return out.str();
}

void Color::reflection_mix(const Color &c, const double refl) {
    r = (1.0 - refl)*r + refl*c.r;
    g = (1.0 - refl)*g + refl*c.g;
    b = (1.0 - refl)*b + refl*c.b;
}

bool Color::operator == (const Color& eq)const {
    return fabs(r - eq.r) < 1. && fabs(g - eq.g) < 1. && fabs(b - eq.b) < 1.;
}

bool Color::operator != (const Color& eq)const {
    return r != eq.r || g != eq.g || b != eq.b;
}

}  // namespace merlict
