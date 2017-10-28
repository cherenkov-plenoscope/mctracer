//======================================================================
// include guard
#ifndef __Color_H_INCLUDED__
#define __Color_H_INCLUDED__

//======================================================================
// forward declared dependencies

//======================================================================
// included dependencies
#include <iostream>
#include <string>
#include <vector>

class Color {

    unsigned char re, gr, bl; 
public:
    
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
    Color(const int r, const int g, const int b);
    Color(const std::vector<Color> &mean);
    unsigned char get_red()const;
    unsigned char get_green()const;
    unsigned char get_blue()const;
    std::string str()const;
    void reflection_mix(const Color &c, const double refl);
    bool operator == (const Color& eq)const;
    bool operator != (const Color& eq)const;
private:

    void assert_is_in_valid_8Bit_range(const int channel)const;
};
#endif // __Color_H_INCLUDED__ 
