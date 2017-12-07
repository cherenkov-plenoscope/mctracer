// Copyright 2014 Sebastian A. Mueller
#include <sstream>
#include <exception>
#include "math.h"
#include "Core/Color.h"

const Color Color::white = Color(255, 255, 255);
const Color Color::sky_blue = Color(127, 188, 255);
const Color Color::gray = Color(128, 128, 128);
const Color Color::dark_gray = Color(25, 25, 25);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(255, 0, 0);
const Color Color::green = Color(0, 255, 0);
const Color Color::blue = Color(0, 0, 255);
const Color Color::grass_green = Color(22, 91, 49);

Color::Color(): r(128), g(128), b(128) {}

Color::Color(const int _r, const int _g, const int _b) {
    assert_is_in_valid_8Bit_range(_r);
    assert_is_in_valid_8Bit_range(_g);
    assert_is_in_valid_8Bit_range(_b);
    r = _r;
    g = _g;
    b = _b;
}

Color::Color(const std::vector<Color> &colors) {
    double _r = 0.0;
    double _g = 0.0;
    double _b = 0.0;

    for (Color c : colors) {
        _r += c.r;
        _g += c.g;
        _b += c.b;
    }
    const double weight = 1.0/colors.size();

    r = (unsigned char)round(_r*weight);
    g = (unsigned char)round(_g*weight);
    b = (unsigned char)round(_b*weight);
}

std::string Color::str()const {
    std::stringstream out;
    out << "(" << static_cast<int>(r);
    out << " " << static_cast<int>(g);
    out << " " << static_cast<int>(b) << ")";
    out << "8 Bit RGB";
    return out.str();
}

void Color::reflection_mix(const Color &c, const double refl) {
    r = static_cast<unsigned char>(
        (1.0 - refl)*static_cast<double>(r) + refl*static_cast<double>(c.r));
    g = static_cast<unsigned char>(
        (1.0 - refl)*static_cast<double>(g) + refl*static_cast<double>(c.g));
    b = static_cast<unsigned char>(
        (1.0 - refl)*static_cast<double>(b) + refl*static_cast<double>(c.b));
}

void Color::assert_is_in_valid_8Bit_range(const int channel)const {
    if ( channel < 0.0 || channel > 255.0 ) {
        std::stringstream info;
        info << "Color::" << __func__ << "()\n";
        info << "Each RGB color channel must be within the valid 8 Bit range\n";
        info << "Expected channels to be: 0 <= channel <= 255, but actual: ";
        info << str() << "\n";
        throw std::out_of_range(info.str());
    }
}

bool Color::operator == (const Color& eq)const {
    return r == eq.r && g == eq.g && b == eq.b;
}

bool Color::operator != (const Color& eq)const {
    return r != eq.r || g != eq.g || b != eq.b;
}
