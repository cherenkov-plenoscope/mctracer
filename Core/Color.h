// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_COLOR_H_
#define MCTRACER_CORE_COLOR_H_

#include <string>
#include <vector>

struct Color {

    unsigned char r, g, b;

    static const Color white;
    static const Color sky_blue;
    static const Color gray;
    static const Color dark_gray;
    static const Color black;
    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color grass_green;

    Color();
    Color(const int _r, const int _g, const int _b);
    Color(const std::vector<Color> &mean);
    std::string str()const;
    void reflection_mix(const Color &c, const double refl);
    bool operator == (const Color& eq)const;
    bool operator != (const Color& eq)const;
    void assert_is_in_valid_8Bit_range(const int channel)const;
};
#endif  // MCTRACER_CORE_COLOR_H_
