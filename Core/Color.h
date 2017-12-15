// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_COLOR_H_
#define CORE_COLOR_H_

#include <string>
#include <vector>

struct Color {
    unsigned char r, g, b;

    static const Color WHITE;
    static const Color SKY_BLUE;
    static const Color GRAY;
    static const Color DARK_GRAY;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color GRASS_GREEN;

    Color();
    Color(const int _r, const int _g, const int _b);
    explicit Color(const std::vector<Color> &mean);
    std::string str()const;
    void reflection_mix(const Color &c, const double refl);
    bool operator == (const Color& eq)const;
    bool operator != (const Color& eq)const;
    void assert_is_in_valid_8Bit_range(const int channel)const;
};
#endif  // CORE_COLOR_H_
