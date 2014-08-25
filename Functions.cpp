#include "Functions.h"

double Deg2Rad(double angle_in_Deg){
	return (M_PI*angle_in_Deg)/180.0;
}

double Rad2Deg(double angle_in_Rad){
	return (angle_in_Rad/M_PI)*180.0;
}

void ClearScreen(){
	std::cout << std::string( 100, '\n' );
}