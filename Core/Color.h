// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_COLOR_H_
#define CORE_COLOR_H_

#include <string>
#include <vector>

struct Color {
    float r, g, b;

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
    Color(float _r, float _g, float _b);
    explicit Color(const std::vector<Color> &mean);
    std::string str()const;
    void reflection_mix(const Color &c, const double refl);
    bool operator == (const Color& eq)const;
    bool operator != (const Color& eq)const;
};
#endif  // CORE_COLOR_H_
