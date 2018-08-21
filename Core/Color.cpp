// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <sstream>
#include <exception>
#include "Core/Color.h"

const Color Color::WHITE = Color(255, 255, 255);
const Color Color::SKY_BLUE = Color(127, 188, 255);
const Color Color::GRAY = Color(128, 128, 128);
const Color Color::DARK_GRAY = Color(25, 25, 25);
const Color Color::BLACK = Color(0, 0, 0);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);
const Color Color::GRASS_GREEN = Color(22, 91, 49);

Color::Color(): r(0), g(0), b(0) {}

Color::Color(float _r, float _g, float _b): r(_r), g(_g), b(_b) {}

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
