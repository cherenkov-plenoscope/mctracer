// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_COLOR_H_
#define CORE_COLOR_H_

#include <string>
#include <vector>

namespace merlict {

struct Color {
    float r, g, b;
    Color(): r(0), g(0), b(0) {}
    Color(float _r, float _g, float _b): r(_r), g(_g), b(_b) {}
    Color(const std::vector<Color> &mean);
    std::string str()const;
    void reflection_mix(const Color &c, const double refl);
    bool operator == (const Color& eq)const;
    bool operator != (const Color& eq)const;
};

const Color COLOR_WHITE = Color(255, 255, 255);
const Color COLOR_SKY_BLUE = Color(127, 188, 255);
const Color COLOR_GRAY = Color(128, 128, 128);
const Color COLOR_DARK_GRAY = Color(25, 25, 25);
const Color COLOR_BLACK = Color(0, 0, 0);
const Color COLOR_RED = Color(255, 0, 0);
const Color COLOR_GREEN = Color(0, 255, 0);
const Color COLOR_BLUE = Color(0, 0, 255);
const Color COLOR_GRASS_GREEN = Color(22, 91, 49);

}  // namespace relleums

#endif  // CORE_COLOR_H_
