#include "Color.h"
const Color Color::syk_blue = Color(127,188,255);
const Color Color::gray = Color(128,128,128);
const Color Color::dark_gray = Color(25,25,25);
//------------------------------------------------------------------------------
Color::Color(){
	set_to_default_color();
}
//------------------------------------------------------------------------------
Color::Color(const int r, const int g, const int b) {
	set_RGB_0to255(r, g, b);
}
//------------------------------------------------------------------------------
Color::Color(const double r, const double g, const double b) {
	set_RGB_0to255(r, g, b);
}
//------------------------------------------------------------------------------
void Color::set_RGB_0to255(
	const int red,
	const int green,
	const int blue
) {
	assert_is_in_valid_8Bit_range(red);
	assert_is_in_valid_8Bit_range(green);
	assert_is_in_valid_8Bit_range(blue);	

	red_0to255 = (double)red;
	green_0to255 = (double)green;
	blue_0to255 = (double)blue;
}
//------------------------------------------------------------------------------
void Color::set_RGB_0to255(
	const double red,
	const double green,
	const double blue
) {
	set_RGB_0to255(
		int(round(red)),
		int(round(green)),
		int(round(blue))
	);
}
//------------------------------------------------------------------------------
unsigned char Color::get_R_as_uchar()const {
	return (unsigned char)(red_0to255);
}	
//------------------------------------------------------------------------------
unsigned char Color::get_G_as_uchar()const {
	return (unsigned char)(green_0to255);
}
//------------------------------------------------------------------------------
unsigned char Color::get_B_as_uchar()const {
	return (unsigned char)(blue_0to255);
}
//------------------------------------------------------------------------------
double Color::get_R_as_double()const {return red_0to255;}
double Color::get_G_as_double()const {return green_0to255;}
double Color::get_B_as_double()const {return blue_0to255;}
//------------------------------------------------------------------------------
std::string Color::get_print()const {
	std::stringstream out;
	out << "(" << red_0to255 << " " << green_0to255 << " " << blue_0to255 << ")";
	out << "8 Bit RGB";
	return out.str();
}
//------------------------------------------------------------------------------
void Color::reflection_mix(
	const Color *c,
	const double refl_coeff
) {
	red_0to255 = (1.0 - refl_coeff)*red_0to255 + refl_coeff*c->red_0to255;
	green_0to255 = (1.0 - refl_coeff)*green_0to255 + refl_coeff*c->green_0to255;
	blue_0to255 = (1.0 - refl_coeff)*blue_0to255 + refl_coeff*c->blue_0to255;
}
//------------------------------------------------------------------------------
void Color::mixture(
	const Color *coulour_to_mix_with,
	const double mixture_coefficient
) {
	red_0to255 = 
		red_0to255 + mixture_coefficient*coulour_to_mix_with->red_0to255;

	green_0to255 = 
		green_0to255 + mixture_coefficient*coulour_to_mix_with->green_0to255;

	blue_0to255 = 
		blue_0to255 + mixture_coefficient*coulour_to_mix_with->blue_0to255;
}
//------------------------------------------------------------------------------
void Color::set_to_default_color() {
	set_RGB_0to255(128, 128, 128);
}
//------------------------------------------------------------------------------
void Color::assert_is_in_valid_8Bit_range(const int channel)const {
	assert_is_in_valid_8Bit_range(double(channel));
}
//------------------------------------------------------------------------------
void Color::assert_is_in_valid_8Bit_range(const double channel)const {
	
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
std::ostream& operator<<(std::ostream& os, const Color& col){
    os << col.get_print();
    return os;
}
