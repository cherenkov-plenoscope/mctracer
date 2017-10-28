#include "Color.h"
#include <sstream>
#include <exception>
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
Color::Color(): re(128), gr(128), bl(128) {}
//------------------------------------------------------------------------------
Color::Color(const int r, const int g, const int b) {
	assert_is_in_valid_8Bit_range(r);
	assert_is_in_valid_8Bit_range(g);
	assert_is_in_valid_8Bit_range(b);	

	re = r;
	gr = g;
	bl = b; 
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
	const double weight = 1.0/colors.size();

	re = (unsigned char)round(r*weight);
	gr = (unsigned char)round(g*weight);
	bl = (unsigned char)round(b*weight); 
}
//------------------------------------------------------------------------------
unsigned char Color::get_red()const {
	return re;
}	
//------------------------------------------------------------------------------
unsigned char Color::get_green()const {
	return gr;
}
//------------------------------------------------------------------------------
unsigned char Color::get_blue()const {
	return bl;
}
//------------------------------------------------------------------------------
std::string Color::str()const {
	std::stringstream out;
	out << "(" << (int)(re);
	out << " " << (int)(gr);
	out << " " << (int)(bl) << ")";
	out << "8 Bit RGB";
	return out.str();
}
//------------------------------------------------------------------------------
void Color::reflection_mix(const Color &c, const double refl) {
	re = (unsigned char)( (1.0 - refl)*double(re) + refl*double(c.re) );
	gr = (unsigned char)( (1.0 - refl)*double(gr) + refl*double(c.gr) );
	bl = (unsigned char)( (1.0 - refl)*double(bl) + refl*double(c.bl) );
}
//------------------------------------------------------------------------------
void Color::assert_is_in_valid_8Bit_range(const int channel)const {
	
	if( channel < 0.0 || channel > 255.0 ) {
		std::stringstream info;
		info << "Color::" << __func__ << "()\n";
		info << "Each RGB color channel must be within the valid 8 Bit range\n";
		info << "Expected channels to be: 0 <= channel <= 255, but actual: ";
		info << str() << "\n";
		throw std::out_of_range(info.str());	
	}
}
//------------------------------------------------------------------------------
bool Color::operator == (const Color& eq)const{
    return re == eq.re && gr == eq.gr && bl == eq.bl;
}
//------------------------------------------------------------------------------
bool Color::operator != (const Color& eq)const {
    return re != eq.re || gr != eq.gr || bl != eq.bl;
}