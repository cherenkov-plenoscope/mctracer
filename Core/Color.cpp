#include "Color.h"
#include "TracerException.h"
#include <sstream>
#include "math.h"
//------------------------------------------------------------------------------
const Color Color::white = Color(255,255,255);
const Color Color::sky_blue = Color(127,188,255);
const Color Color::gray = Color(128,128,128);
const Color Color::dark_gray = Color(25,25,25);
const Color Color::black = Color(0,0,0);
const Color Color::red = Color(255,0,0);
const Color Color::green = Color(0,255,0);
const Color Color::blue = Color(0,0,255);
const Color Color::grass_green = Color(22,91,49);
//------------------------------------------------------------------------------
Color::Color(){
	*this = Color(128, 128, 128);
}
//------------------------------------------------------------------------------
Color::Color(const int r, const int g, const int b) {
	assert_is_in_valid_8Bit_range(r);
	assert_is_in_valid_8Bit_range(g);
	assert_is_in_valid_8Bit_range(b);	

	c_red = r;
	c_green = g;
	c_blue = b; 
}
//------------------------------------------------------------------------------
Color::Color(const std::vector<Color> &colors) {
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;

	for(Color c: colors) {
		r = r + c.get_red();
		g = g + c.get_green();
		b = b + c.get_blue();
	}
	double weight = 1.0/colors.size();

	c_red = (unsigned char)round(r*weight);
	c_green = (unsigned char)round(g*weight);
	c_blue = (unsigned char)round(b*weight); 
}
//------------------------------------------------------------------------------
unsigned char Color::get_red()const {
	return c_red;
}	
//------------------------------------------------------------------------------
unsigned char Color::get_green()const {
	return c_green;
}
//------------------------------------------------------------------------------
unsigned char Color::get_blue()const {
	return c_blue;
}
//------------------------------------------------------------------------------
std::string Color::get_print()const {
	std::stringstream out;
	out << "(" << (int)(c_red);
	out << " " << (int)(c_green);
	out << " " << (int)(c_blue) << ")";
	out << "8 Bit RGB";
	return out.str();
}
//------------------------------------------------------------------------------
void Color::reflection_mix(const Color &c, const double refl) {
	c_red = (unsigned char)( (1.0 - refl)*double(c_red) + refl*double(c.c_red) );
	c_green = (unsigned char)( (1.0 - refl)*double(c_green) + refl*double(c.c_green) );
	c_blue = (unsigned char)( (1.0 - refl)*double(c_blue) + refl*double(c.c_blue) );
}
//------------------------------------------------------------------------------
void Color::assert_is_in_valid_8Bit_range(const int channel)const {
	
	if( channel < 0.0 || channel > 255.0 ) {
		std::stringstream info;
		info << "Color::" << __func__ << "()\n";
		info << "Each RGB color channel must be within the valid 8 Bit range\n";
		info << "Expected channels to be: 0 <= channel <= 255, but actual: ";
		info << get_print() << "\n";
		throw TracerException(info.str());	
	}
}
//------------------------------------------------------------------------------
bool Color::operator == (const Color& eq)const{
    return c_red == eq.c_red && c_green == eq.c_green && c_blue == eq.c_blue;
}
//------------------------------------------------------------------------------
bool Color::operator != (const Color& eq)const {
    return c_red != eq.c_red || c_green != eq.c_green || c_blue != eq.c_blue;
}